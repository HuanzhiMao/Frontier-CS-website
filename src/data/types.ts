// Frontier-CS Data Types

export interface Problem {
  name: string;
  category: string;
  displayName: string;
}

export interface BenchmarkResult {
  solution: string;
  problem: string;
  model: string;
  score: number | null;
  error: string | null;
}

export interface Model {
  name: string;
  provider: string;
  status: string;
  totalProblems?: number;
  attempted?: number;
  avgScore?: number;
  results?: BenchmarkResult[];
}

export interface ResultsData {
  results: BenchmarkResult[];
  problems: Problem[];
  models: Model[];
}

export interface LeaderboardModel extends Model {
  totalProblems: number;
  attempted: number;
  avgScore: number;
  results: BenchmarkResult[];
}

export type CategoryName =
  | 'Operating Systems & Distributed Systems'
  | 'HPC / GPU Systems'
  | 'Artificial Intelligence'
  | 'Databases & Data Infrastructure'
  | 'Other';

export interface CategoryStats {
  category: CategoryName;
  problems: Problem[];
  totalProblems: number;
}

// Competitive Programming Types

export interface CPTestCase {
  ok: boolean;
  status: string;
  time: number;
  memory: number;
  msg: string;
  scoreRatio: number;
}

export interface CPResult {
  problem: string;
  shot: number;
  model: string;
  status: string;
  passed: boolean;
  result: string;
  score: number | null;
  cases: number;
  passedCases: number;
  file: string;
}

export interface CPProblem {
  name: string;
  category: string;
  displayName: string;
}

export interface CPModel {
  name: string;
  provider: string;
  status: string;
  totalProblems: number;
  uniqueProblemsSolved: number;
  totalAttempts: number;
  passedAttempts: number;
  avgScore: number;
  passRate: number;
}

export interface CPResultsData {
  results: CPResult[];
  problems: CPProblem[];
  models: Model[];
  bestResults: CPResult[];
}

export type CPCategoryName =
  | 'AtCoder Heuristic Contest'
  | 'LLM Coding Challenge'
  | 'Competitive Programming';
