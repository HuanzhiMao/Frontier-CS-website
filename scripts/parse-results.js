import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// Parse the summary.txt file
function parseSummary(summaryPath) {
  const content = fs.readFileSync(summaryPath, 'utf-8');
  const lines = content.split('\n');

  const results = [];
  const problemsMap = new Map();
  const modelsMap = new Map();

  for (let i = 0; i < lines.length; i++) {
    const line = lines[i].trim();

    // Skip empty lines and headers
    if (!line || line.includes('Evaluation Summary') || line.includes('===')) {
      continue;
    }

    // Parse line format: "solution_name -> problem_name:"
    // Next line: "Score: X"
    const match = line.match(/^(.+?)\s*->\s*(.+?):$/);
    if (!match) continue;

    const [, solutionName, problemName] = match;

    // Get next line for score
    i++;
    if (i >= lines.length) continue;

    const scoreLine = lines[i].trim();
    const scoreMatch = scoreLine.match(/^Score:\s*(.+)$/);
    if (!scoreMatch) continue;

    const scoreStr = scoreMatch[1];

    // Extract model from solution name - ONLY keep GPT-5 and Gemini 2.5 Pro
    let model = null;
    if (solutionName.includes('gemini_2_5_pro')) {
      model = 'Gemini';
    } else if (solutionName.includes('gpt_5') && !solutionName.includes('gpt_5_pro')) {
      model = 'GPT';
    }

    // Skip if not one of the allowed models
    if (!model) continue;

    // Parse score - treat non-numeric as 0
    let score = 0;
    const parsedScore = parseFloat(scoreStr);
    if (!isNaN(parsedScore)) {
      score = parsedScore;
    }
    // If not a valid number, score stays 0

    // Store result
    results.push({
      solution: solutionName,
      problem: problemName,
      model,
      score,
      type: 'research'
    });

    // Track unique problems and models
    if (!problemsMap.has(problemName)) {
      // Extract category from problem path
      let category = 'Other';
      if (problemName.includes('cant_be_late')) {
        category = 'Operating Systems & Distributed Systems';
      } else if (problemName.includes('gemm') || problemName.includes('attn') || problemName.includes('vector_addition') || problemName.includes('mamba')) {
        category = 'HPC / GPU Systems';
      } else if (problemName.includes('imagenet') || problemName.includes('cloudcast') || problemName.includes('cross_entropy') || problemName.includes('linear_regression') || problemName.includes('poc_generation')) {
        category = 'Artificial Intelligence';
      } else if (problemName.includes('vdb') || problemName.includes('llm_sql')) {
        category = 'Databases & Data Infrastructure';
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
        provider: model === 'Gemini' ? 'Google' : model === 'GPT' ? 'OpenAI' : 'Unknown',
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
  const summaryPath = path.join(__dirname, '../Frontier-CS/results/summary.txt');
  const outputDir = path.join(__dirname, '../public/data');

  // Ensure output directory exists
  if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir, { recursive: true });
  }

  console.log('Parsing research problems...');
  const data = parseSummary(summaryPath);

  console.log(`Found ${data.results.length} results`);
  console.log(`Found ${data.problems.length} unique problems`);
  console.log(`Found ${data.models.length} unique models`);

  // Write JSON files
  const resultsPath = path.join(outputDir, 'research-results.json');
  fs.writeFileSync(resultsPath, JSON.stringify(data, null, 2));
  console.log(`\nWrote results to ${resultsPath}`);

  // Create summary by model and problem
  const leaderboard = data.models.map(model => {
    const modelResults = data.results.filter(r => r.model === model.name);
    const validScores = modelResults.map(r => r.score);
    const avgScore = validScores.length > 0
      ? validScores.reduce((a, b) => a + b, 0) / validScores.length
      : 0;

    return {
      ...model,
      type: 'research',
      totalProblems: data.problems.length,
      attempted: modelResults.length,
      avgScore: Number(avgScore.toFixed(2)),
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
