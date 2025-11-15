import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// Parse all competitive programming results
function parseCPResults(solutionsPath) {
  const results = [];
  const problemsMap = new Map();
  const modelsMap = new Map();

  // Get all result JSON files
  const files = fs.readdirSync(solutionsPath).filter(f => f.endsWith('_result.json'));

  console.log(`Found ${files.length} result files`);

  for (const file of files) {
    try {
      // Parse filename: {problem}_{shot}_{model}_result.json
      const match = file.match(/^(.+?)_(\d+)_(claude|gemini|gpt|grok)_result\.json$/);
      if (!match) {
        console.warn(`Skipping file with unexpected format: ${file}`);
        continue;
      }

      const [, problem, shot, model] = match;

      // Read and parse JSON
      const filePath = path.join(solutionsPath, file);
      const content = fs.readFileSync(filePath, 'utf-8');
      const data = JSON.parse(content);

      // Map model names to display names
      const modelDisplayNames = {
        'claude': 'Claude-Sonnet-4-5-20250929 (FC)',
        'gemini': 'Gemini-2.5-Pro (FC)',
        'gpt': 'GPT-5-2025-08-07 (FC)',
        'grok': 'Grok-Code-Fast-1 (FC)'
      };

      const modelName = modelDisplayNames[model] || model;

      // Determine problem category - only 2 categories
      let category = 'Self-Designed';
      if (problem.startsWith('ahc')) {
        category = 'AtCoder';
      }

      // Parse score - treat non-numeric as 0
      let score = 0;
      if (data.score !== undefined && data.score !== null) {
        const parsedScore = parseFloat(data.score);
        if (!isNaN(parsedScore)) {
          score = parsedScore;
        }
      }

      // Store result
      results.push({
        problem,
        shot: parseInt(shot),
        model: modelName,
        status: data.status || 'unknown',
        passed: data.passed || false,
        result: data.result || 'N/A',
        score,
        cases: data.cases ? data.cases.length : 0,
        passedCases: data.cases ? data.cases.filter(c => c.ok).length : 0,
        file,
        type: 'competitive'
      });

      // Track unique problems
      if (!problemsMap.has(problem)) {
        problemsMap.set(problem, {
          name: problem,
          category,
          displayName: problem.replace(/_/g, ' ').toUpperCase()
        });
      }

      // Track unique models
      if (!modelsMap.has(modelName)) {
        modelsMap.set(modelName, {
          name: modelName,
          provider: modelName.includes('GPT') ? 'OpenAI' :
                    modelName.includes('Claude') ? 'Anthropic' :
                    modelName.includes('Gemini') ? 'Google' :
                    modelName.includes('Grok') ? 'xAI' : 'Unknown',
          license: modelName.includes('GPT') ? 'Proprietary' :
                   modelName.includes('Claude') ? 'Proprietary' :
                   modelName.includes('Gemini') ? 'Proprietary' :
                   modelName.includes('Grok') ? 'Proprietary' : 'Unknown',
          status: 'active'
        });
      }
    } catch (error) {
      console.error(`Error parsing ${file}:`, error.message);
    }
  }

  return {
    results,
    problems: Array.from(problemsMap.values()),
    models: Array.from(modelsMap.values())
  };
}

// Aggregate results by model
function aggregateByModel(data) {
  const modelStats = new Map();

  for (const result of data.results) {
    if (!modelStats.has(result.model)) {
      modelStats.set(result.model, {
        model: result.model,
        totalAttempts: 0,
        totalProblems: new Set(),
        scores: [],
        passed: 0,
        avgScore: 0
      });
    }

    const stats = modelStats.get(result.model);
    stats.totalAttempts++;
    stats.totalProblems.add(result.problem);

    // Score is always a number (0 if invalid)
    stats.scores.push(result.score);

    if (result.passed) {
      stats.passed++;
    }
  }

  // Calculate averages with category breakdowns
  const leaderboard = Array.from(modelStats.values()).map(stats => {
    const avgScore = stats.scores.length > 0
      ? stats.scores.reduce((a, b) => a + b, 0) / stats.scores.length
      : 0;

    const modelInfo = data.models.find(m => m.name === stats.model);

    // Calculate category scores
    const categories = ['AtCoder', 'Self-Designed'];
    const categoryScores = {};

    categories.forEach(cat => {
      const catProblems = data.problems.filter(p => p.category === cat);
      const catResults = data.results.filter(r => {
        const problem = data.problems.find(p => p.name === r.problem);
        return r.model === stats.model && problem && problem.category === cat;
      });

      if (catResults.length > 0) {
        const catAvg = catResults.reduce((sum, r) => sum + r.score, 0) / catResults.length;
        categoryScores[cat] = {
          avgScore: Number(catAvg.toFixed(2)),
          attempts: catResults.length,
          totalProblems: catProblems.length
        };
      }
    });

    return {
      name: stats.model,
      provider: modelInfo?.provider || 'Unknown',
      license: modelInfo?.license || 'Unknown',
      status: 'active',
      totalProblems: data.problems.length,
      uniqueProblemsSolved: stats.totalProblems.size,
      totalAttempts: stats.totalAttempts,
      passedAttempts: stats.passed,
      avgScore: Number(avgScore.toFixed(2)),
      passRate: Number((stats.passed / stats.totalAttempts * 100).toFixed(2)),
      categoryScores
    };
  }).sort((a, b) => b.avgScore - a.avgScore);

  return leaderboard;
}

// Aggregate best results per problem-model pair (best shot)
function aggregateBestResults(data) {
  const bestResults = new Map();

  for (const result of data.results) {
    const key = `${result.problem}_${result.model}`;

    // Keep the result with the highest score
    if (!bestResults.has(key) || result.score > bestResults.get(key).score) {
      bestResults.set(key, result);
    }
  }

  return Array.from(bestResults.values());
}

// Main execution
try {
  const solutionsPath = path.join(__dirname, '../solutions');
  const outputDir = path.join(__dirname, '../public/data');

  // Ensure output directory exists
  if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir, { recursive: true });
  }

  console.log('Parsing competitive programming results...');
  const data = parseCPResults(solutionsPath);

  console.log(`Found ${data.results.length} results`);
  console.log(`Found ${data.problems.length} unique problems`);
  console.log(`Found ${data.models.length} unique models`);

  // Aggregate data
  const leaderboard = aggregateByModel(data);
  const bestResults = aggregateBestResults(data);

  // Write JSON files
  const cpResultsPath = path.join(outputDir, 'cp-results.json');
  fs.writeFileSync(cpResultsPath, JSON.stringify({
    ...data,
    bestResults
  }, null, 2));
  console.log(`\nWrote results to ${cpResultsPath}`);

  const cpLeaderboardPath = path.join(outputDir, 'cp-leaderboard.json');
  fs.writeFileSync(cpLeaderboardPath, JSON.stringify(leaderboard, null, 2));
  console.log(`Wrote leaderboard to ${cpLeaderboardPath}`);

  // Print summary
  console.log('\n📊 Leaderboard Summary:');
  leaderboard.forEach((model, index) => {
    console.log(`${index + 1}. ${model.name}: ${model.avgScore} avg score, ${model.passRate}% pass rate (${model.totalAttempts} attempts)`);
  });

  console.log('\n✅ Parsing complete!');

} catch (error) {
  console.error('Error parsing competitive programming results:', error);
  process.exit(1);
}
