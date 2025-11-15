import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// Parse the results.csv file
function parseResultsCSV(csvPath) {
  const content = fs.readFileSync(csvPath, 'utf-8');
  const lines = content.split('\n');

  const results = [];
  const problemsMap = new Map();
  const modelsMap = new Map();

  // Map model names from CSV to display names
  const modelDisplayNames = {
    'gpt5': 'GPT-5-2025-08-07 (FC)',
    'gemini2.5pro': 'Gemini-2.5-Pro (FC)',
    'claude4.5sonnet': 'Claude-Sonnet-4-5-20250929 (FC)',
    'grokcodefast1': 'Grok-Code-Fast-1 (FC)'
  };

  // Skip header line
  for (let i = 1; i < lines.length; i++) {
    const line = lines[i].trim();
    if (!line) continue;

    // Parse CSV: solution,problem,score,status,timestamp
    const parts = line.split(',');
    if (parts.length < 4) continue;

    const solutionName = parts[0];
    const problemName = parts[1];
    const scoreStr = parts[2];
    const status = parts[3];

    // Skip baseline solutions
    if (solutionName.includes('baseline')) continue;

    // Extract model from solution name
    let modelKey = null;
    if (solutionName.startsWith('gpt5_')) {
      modelKey = 'gpt5';
    } else if (solutionName.startsWith('gemini2.5pro_')) {
      modelKey = 'gemini2.5pro';
    } else if (solutionName.startsWith('claude4.5sonnet_')) {
      modelKey = 'claude4.5sonnet';
    } else if (solutionName.startsWith('grokcodefast1_')) {
      modelKey = 'grokcodefast1';
    }

    // Skip if model not recognized
    if (!modelKey) continue;

    const model = modelDisplayNames[modelKey];

    // Parse score - treat ALL non-numeric values as 0, cap negative scores at 0
    let score = 0;
    const parsedScore = parseFloat(scoreStr);
    if (!isNaN(parsedScore) && isFinite(parsedScore)) {
      score = Math.max(0, parsedScore); // Cap negative scores at 0
    }
    // All error statuses, N/A, or non-numeric values become 0

    // Store result
    results.push({
      solution: solutionName,
      problem: problemName,
      model,
      score,
      status,
      type: 'research'
    });

    // Track unique problems and models
    if (!problemsMap.has(problemName)) {
      // Extract category from problem path for research problems
      let category = 'Other';
      if (problemName.includes('cant_be_late')) {
        category = 'Systems';
      } else if (problemName.includes('gemm') || problemName.includes('attn') || problemName.includes('vector_addition') || problemName.includes('mamba') || problemName.includes('group_gemm') || problemName.includes('mixed_gemm')) {
        category = 'HPC';
      } else if (problemName.includes('imagenet') || problemName.includes('cloudcast') || problemName.includes('cross_entropy') || problemName.includes('linear_regression') || problemName.includes('poc_generation') || problemName.includes('ragged_attention')) {
        category = 'AI';
      } else if (problemName.includes('vdb') || problemName.includes('llm_sql')) {
        category = 'Database';
      }

      problemsMap.set(problemName, {
        name: problemName,
        category,
        type: 'research',
        displayName: problemName.split('/').pop().replace(/_/g, ' ').replace(/\b\w/g, c => c.toUpperCase())
      });
    }

    if (!modelsMap.has(model)) {
      modelsMap.set(model, {
        name: model,
        provider: model.includes('GPT') ? 'OpenAI' :
                  model.includes('Gemini') ? 'Google' :
                  model.includes('Claude') ? 'Anthropic' :
                  model.includes('Grok') ? 'xAI' : 'Unknown',
        license: 'Proprietary',
        status: 'active'
      });
    }
  }

  return {
    results,
    problems: Array.from(problemsMap.values()),
    models: Array.from(modelsMap.values())
  };
}

// Main execution
try {
  const csvPath = path.join(__dirname, '../Frontier-CS/results/results.csv');
  const outputDir = path.join(__dirname, '../public/data');

  // Ensure output directory exists
  if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir, { recursive: true });
  }

  console.log('Parsing research problems from results.csv...');
  const data = parseResultsCSV(csvPath);

  console.log(`Found ${data.results.length} results`);
  console.log(`Found ${data.problems.length} unique problems`);
  console.log(`Found ${data.models.length} unique models`);

  // Write JSON files
  const resultsPath = path.join(outputDir, 'research-results.json');
  fs.writeFileSync(resultsPath, JSON.stringify(data, null, 2));
  console.log(`\nWrote results to ${resultsPath}`);

  // Create summary by model with category breakdowns
  const leaderboard = data.models.map(model => {
    const modelResults = data.results.filter(r => r.model === model.name);

    // Calculate overall score
    const validScores = modelResults.map(r => r.score);
    const avgScore = validScores.length > 0
      ? Math.max(0, validScores.reduce((a, b) => a + b, 0) / validScores.length)
      : 0;

    // Calculate category scores
    const categories = ['Systems', 'HPC', 'AI', 'Database'];
    const categoryScores = {};

    categories.forEach(cat => {
      const catProblems = data.problems.filter(p => p.category === cat);
      const catResults = modelResults.filter(r => {
        const problem = data.problems.find(p => p.name === r.problem);
        return problem && problem.category === cat;
      });

      if (catResults.length > 0) {
        const catAvg = catResults.reduce((sum, r) => sum + r.score, 0) / catResults.length;
        categoryScores[cat] = {
          avgScore: Number(Math.max(0, catAvg).toFixed(2)), // Cap at 0
          attempted: catResults.length,
          totalProblems: catProblems.length
        };
      }
    });

    return {
      ...model,
      type: 'research',
      totalProblems: data.problems.length,
      attempted: modelResults.length,
      avgScore: Number(avgScore.toFixed(2)),
      categoryScores,
      results: modelResults
    };
  }).sort((a, b) => b.avgScore - a.avgScore);

  const leaderboardPath = path.join(outputDir, 'research-leaderboard.json');
  fs.writeFileSync(leaderboardPath, JSON.stringify(leaderboard, null, 2));
  console.log(`Wrote leaderboard to ${leaderboardPath}`);

  console.log('\n✅ Research problems parsing complete!');

} catch (error) {
  console.error('Error parsing results:', error);
  process.exit(1);
}
