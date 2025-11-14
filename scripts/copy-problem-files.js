import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const sourceBase = path.join(__dirname, '../Frontier-CS/competitive_programming');
const destProblemsDir = path.join(__dirname, '../public/problems');
const sourceSolutionsDir = path.join(__dirname, '../solutions');
const destSolutionsDir = path.join(__dirname, '../public/solutions');

// Ensure destination directories exist
if (!fs.existsSync(destProblemsDir)) {
  fs.mkdirSync(destProblemsDir, { recursive: true });
}

if (!fs.existsSync(destSolutionsDir)) {
  fs.mkdirSync(destSolutionsDir, { recursive: true });
}

console.log('Copying competitive programming problem files...\n');

// Get all problem directories
const problemDirs = fs.readdirSync(sourceBase).filter(name => {
  const fullPath = path.join(sourceBase, name);
  return fs.statSync(fullPath).isDirectory();
});

let copiedCount = 0;

// Copy problem statements and ground truth solutions
for (const problemDir of problemDirs) {
  const sourceProblemPath = path.join(sourceBase, problemDir);
  const destProblemPath = path.join(destProblemsDir, problemDir);

  if (!fs.existsSync(destProblemPath)) {
    fs.mkdirSync(destProblemPath, { recursive: true });
  }

  // Copy statement.txt if it exists
  const statementPath = path.join(sourceProblemPath, 'statement.txt');
  if (fs.existsSync(statementPath)) {
    fs.copyFileSync(statementPath, path.join(destProblemPath, 'statement.txt'));
    copiedCount++;
  }

  // Copy ground truth solution (look for .cpp files)
  const files = fs.readdirSync(sourceProblemPath);
  const groundTruthFile = files.find(f => f.endsWith('.cpp') && !f.includes('base') && !f.includes('chatgpt'));

  if (groundTruthFile) {
    fs.copyFileSync(
      path.join(sourceProblemPath, groundTruthFile),
      path.join(destProblemPath, 'solution.cpp')
    );
    copiedCount++;
  }
}

console.log(`Copied ${copiedCount} problem files (statements and ground truth solutions)`);

// Copy model solution files
console.log('\nCopying model solution files...\n');

const solutionFiles = fs.readdirSync(sourceSolutionsDir).filter(f =>
  f.endsWith('_solution.cpp') || f.endsWith('_result.json')
);

let solutionCount = 0;
for (const file of solutionFiles) {
  fs.copyFileSync(
    path.join(sourceSolutionsDir, file),
    path.join(destSolutionsDir, file)
  );
  solutionCount++;
}

console.log(`Copied ${solutionCount} model solution and result files`);

console.log('\n✅ File copying complete!');
