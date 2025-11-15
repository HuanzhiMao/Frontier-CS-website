import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// Hardcoded competitive programming leaderboard data
const cpLeaderboard = [
  {
    name: 'Gemini-2.5-Pro (FC)',
    provider: 'Google',
    license: 'Proprietary',
    status: 'active',
    avgScore: 11.69, // Use Score@1 as the primary score for sorting
    categoryScores: {
      'Avg@5': { avgScore: 11.03 },
      'Score@5': { avgScore: 24.10 },
      'Pass@1': { avgScore: 36.19 },
      'Pass@5': { avgScore: 60.00 }
    }
  },
  {
    name: 'GPT-5-2025-08-07 (FC)',
    provider: 'OpenAI',
    license: 'Proprietary',
    status: 'active',
    avgScore: 9.81,
    categoryScores: {
      'Avg@5': { avgScore: 10.70 },
      'Score@5': { avgScore: 22.23 },
      'Pass@1': { avgScore: 35.24 },
      'Pass@5': { avgScore: 60.95 }
    }
  },
  {
    name: 'Grok-Code-Fast-1 (FC)',
    provider: 'xAI',
    license: 'Proprietary',
    status: 'active',
    avgScore: 6.32,
    categoryScores: {
      'Avg@5': { avgScore: 8.75 },
      'Score@5': { avgScore: 18.97 },
      'Pass@1': { avgScore: 17.76 },
      'Pass@5': { avgScore: 46.73 }
    }
  },
  {
    name: 'Claude-Opus-4-1 (FC)',
    provider: 'Anthropic',
    license: 'Proprietary',
    status: 'active',
    avgScore: 6.10,
    categoryScores: {
      'Avg@5': { avgScore: 5.40 },
      'Score@5': { avgScore: 12.50 },
      'Pass@1': { avgScore: 28.04 },
      'Pass@5': { avgScore: 44.86 }
    }
  },
  {
    name: 'Claude-Sonnet-4-5-20250929 (FC)',
    provider: 'Anthropic',
    license: 'Proprietary',
    status: 'active',
    avgScore: 3.91,
    categoryScores: {
      'Avg@5': { avgScore: 6.20 },
      'Score@5': { avgScore: 13.50 },
      'Pass@1': { avgScore: 21.50 },
      'Pass@5': { avgScore: 48.60 }
    }
  }
].sort((a, b) => b.avgScore - a.avgScore);

// Main execution
try {
  const outputDir = path.join(__dirname, '../public/data');

  // Ensure output directory exists
  if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir, { recursive: true });
  }

  console.log('Generating competitive programming leaderboard...');

  const cpLeaderboardPath = path.join(outputDir, 'cp-leaderboard.json');
  fs.writeFileSync(cpLeaderboardPath, JSON.stringify(cpLeaderboard, null, 2));
  console.log(`Wrote leaderboard to ${cpLeaderboardPath}`);

  // Print summary
  console.log('\n📊 CP Leaderboard Summary:');
  cpLeaderboard.forEach((model, index) => {
    console.log(`${index + 1}. ${model.name}: Score@1=${model.avgScore}, Pass@1=${model.categoryScores['Pass@1'].avgScore}%`);
  });

  console.log('\n✅ CP leaderboard generation complete!');

} catch (error) {
  console.error('Error generating CP leaderboard:', error);
  process.exit(1);
}
