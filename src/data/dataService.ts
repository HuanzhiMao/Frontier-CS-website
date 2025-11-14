import type {
  ResultsData,
  LeaderboardModel,
  Problem,
  BenchmarkResult,
  CategoryName,
  CategoryStats,
  CPResultsData,
  CPModel
} from './types';

class DataService {
  private resultsCache: ResultsData | null = null;
  private leaderboardCache: LeaderboardModel[] | null = null;
  private cpResultsCache: CPResultsData | null = null;
  private cpLeaderboardCache: CPModel[] | null = null;

  /**
   * Load all results data (research problems)
   */
  async loadResults(): Promise<ResultsData> {
    if (this.resultsCache) {
      return this.resultsCache;
    }

    const response = await fetch('/data/research-results.json');
    if (!response.ok) {
      throw new Error('Failed to load results data');
    }

    this.resultsCache = await response.json();
    return this.resultsCache!;
  }

  /**
   * Load leaderboard data (research problems)
   */
  async loadLeaderboard(): Promise<LeaderboardModel[]> {
    if (this.leaderboardCache) {
      return this.leaderboardCache;
    }

    const response = await fetch('/data/research-leaderboard.json');
    if (!response.ok) {
      throw new Error('Failed to load leaderboard data');
    }

    this.leaderboardCache = await response.json();
    return this.leaderboardCache!;
  }

  /**
   * Get all problems
   */
  async getProblems(): Promise<Problem[]> {
    const data = await this.loadResults();
    return data.problems;
  }

  /**
   * Get problems by category
   */
  async getProblemsByCategory(category: CategoryName): Promise<Problem[]> {
    const data = await this.loadResults();
    return data.problems.filter(p => p.category === category);
  }

  /**
   * Get category statistics
   */
  async getCategoryStats(): Promise<CategoryStats[]> {
    const data = await this.loadResults();
    const categoriesMap = new Map<CategoryName, Problem[]>();

    data.problems.forEach(problem => {
      const cat = problem.category as CategoryName;
      if (!categoriesMap.has(cat)) {
        categoriesMap.set(cat, []);
      }
      categoriesMap.get(cat)!.push(problem);
    });

    return Array.from(categoriesMap.entries()).map(([category, problems]) => ({
      category,
      problems,
      totalProblems: problems.length
    }));
  }

  /**
   * Get results for a specific model
   */
  async getModelResults(modelName: string): Promise<BenchmarkResult[]> {
    const data = await this.loadResults();
    return data.results.filter(r => r.model === modelName);
  }

  /**
   * Get results for a specific problem
   */
  async getProblemResults(problemName: string): Promise<BenchmarkResult[]> {
    const data = await this.loadResults();
    return data.results.filter(r => r.problem === problemName);
  }

  /**
   * Get model performance by category
   */
  async getModelPerformanceByCategory(modelName: string): Promise<Map<CategoryName, number>> {
    const data = await this.loadResults();
    const modelResults = data.results.filter(r => r.model === modelName && r.score !== null);

    const categoryScores = new Map<CategoryName, number[]>();

    modelResults.forEach(result => {
      const problem = data.problems.find(p => p.name === result.problem);
      if (!problem) return;

      const cat = problem.category as CategoryName;
      if (!categoryScores.has(cat)) {
        categoryScores.set(cat, []);
      }
      if (result.score !== null) {
        categoryScores.get(cat)!.push(result.score);
      }
    });

    const categoryAvgs = new Map<CategoryName, number>();
    categoryScores.forEach((scores, category) => {
      const avg = scores.length > 0 ? scores.reduce((a, b) => a + b, 0) / scores.length : 0;
      categoryAvgs.set(category, avg);
    });

    return categoryAvgs;
  }

  /**
   * Get top models for a specific problem
   */
  async getTopModelsForProblem(problemName: string, limit: number = 5): Promise<BenchmarkResult[]> {
    const results = await this.getProblemResults(problemName);
    return results
      .filter(r => r.score !== null)
      .sort((a, b) => (b.score || 0) - (a.score || 0))
      .slice(0, limit);
  }

  /**
   * Load competitive programming results
   */
  async loadCPResults(): Promise<CPResultsData> {
    if (this.cpResultsCache) {
      return this.cpResultsCache;
    }

    const response = await fetch('/data/cp-results.json');
    if (!response.ok) {
      throw new Error('Failed to load competitive programming results');
    }

    this.cpResultsCache = await response.json();
    return this.cpResultsCache!;
  }

  /**
   * Load competitive programming leaderboard
   */
  async loadCPLeaderboard(): Promise<CPModel[]> {
    if (this.cpLeaderboardCache) {
      return this.cpLeaderboardCache;
    }

    const response = await fetch('/data/cp-leaderboard.json');
    if (!response.ok) {
      throw new Error('Failed to load competitive programming leaderboard');
    }

    this.cpLeaderboardCache = await response.json();
    return this.cpLeaderboardCache!;
  }

  /**
   * Clear cache
   */
  clearCache(): void {
    this.resultsCache = null;
    this.leaderboardCache = null;
    this.cpResultsCache = null;
    this.cpLeaderboardCache = null;
  }
}

// Export singleton instance
export const dataService = new DataService();
