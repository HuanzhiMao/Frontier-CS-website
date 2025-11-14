import React, { useMemo, useState } from 'react';
import {
  Alert,
  Box,
  Button,
  Chip,
  CircularProgress,
  Paper,
  Stack,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow,
  Typography,
} from '@mui/material';
import { useRequest } from 'ahooks';
import { dataService } from '../data';
import type { CPResult } from '../data/types';
import { ProblemDetailModal } from './ProblemDetailModal';

interface ProblemExample {
  problem: string;
  displayName: string;
  category: string;
  solutions: {
    model: string;
    status: string;
    passed: boolean;
    score: number;
    passedCases: number;
    totalCases: number;
  }[];
}

// Seeded random number generator for consistent sampling
function seededRandom(seed: number) {
  let s = seed;
  return function() {
    s = Math.sin(s) * 10000;
    return s - Math.floor(s);
  };
}

export const ExampleProblems: React.FC = () => {
  const [selectedProblem, setSelectedProblem] = useState<{ problem: string; displayName: string } | null>(null);

  const { data: cpResults, loading, error } = useRequest(async () => {
    return await dataService.loadCPResults();
  });

  // Sample random problems (fixed seed for consistency)
  const examples = useMemo((): ProblemExample[] => {
    if (!cpResults) return [];

    const { results, problems } = cpResults;

    // Use seed for consistent random sampling
    const random = seededRandom(12345);

    // Group results by problem
    const problemResultsMap = new Map<string, CPResult[]>();
    results.forEach(result => {
      if (!problemResultsMap.has(result.problem)) {
        problemResultsMap.set(result.problem, []);
      }
      problemResultsMap.get(result.problem)!.push(result);
    });

    // Get problems that have results from all models
    const completeProblems = Array.from(problemResultsMap.entries())
      .filter(([_, results]) => {
        const models = new Set(results.map(r => r.model));
        return models.size >= 3; // At least 3 different models
      })
      .map(([problem, _]) => problem);

    // Shuffle with seeded random
    const shuffled = completeProblems
      .map(p => ({ problem: p, sort: random() }))
      .sort((a, b) => a.sort - b.sort)
      .map(({ problem }) => problem);

    // Take first 5 problems
    const sampledProblems = shuffled.slice(0, 5);

    // Build examples
    return sampledProblems.map(problemName => {
      const problemInfo = problems.find(p => p.name === problemName);
      const problemResults = problemResultsMap.get(problemName) || [];

      // Get best result per model
      const bestByModel = new Map<string, CPResult>();
      problemResults.forEach(result => {
        const existing = bestByModel.get(result.model);
        if (!existing || (result.score ?? 0) > (existing.score ?? 0)) {
          bestByModel.set(result.model, result);
        }
      });

      const solutions = Array.from(bestByModel.values())
        .sort((a, b) => (b.score ?? 0) - (a.score ?? 0))
        .map(result => ({
          model: result.model,
          status: result.status,
          passed: result.passed,
          score: result.score ?? 0,
          passedCases: result.passedCases,
          totalCases: result.cases,
        }));

      return {
        problem: problemName,
        displayName: problemInfo?.displayName || problemName.replace(/_/g, ' ').toUpperCase(),
        category: problemInfo?.category || 'Unknown',
        solutions,
      };
    });
  }, [cpResults]);

  if (loading) {
    return (
      <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', minHeight: 300 }}>
        <CircularProgress />
      </Box>
    );
  }

  if (error) {
    return <Alert severity="error">Failed to load example problems: {error.message}</Alert>;
  }

  const getStatusColor = (passed: boolean, status: string) => {
    if (passed) return 'success';
    if (status === 'Wrong Answer') return 'warning';
    return 'error';
  };

  return (
    <Paper sx={{ p: { xs: 2, md: 3 } }}>
      <Stack spacing={3}>
        <Stack spacing={0.75}>
          <Typography variant="h6" sx={{ fontWeight: 600 }}>
            Example Problems
          </Typography>
          <Typography variant="caption" sx={{ color: 'text.secondary' }}>
            Randomly sampled competitive programming problems showing model performance
          </Typography>
        </Stack>

        <Stack spacing={3}>
          {examples.map((example, idx) => (
            <Box key={example.problem}>
              <Stack spacing={1.5}>
                <Stack direction="row" spacing={1.5} alignItems="center" flexWrap="wrap">
                  <Typography variant="subtitle1" sx={{ fontWeight: 600 }}>
                    {idx + 1}. {example.displayName}
                  </Typography>
                  <Chip
                    label={example.category}
                    size="small"
                    sx={{
                      height: 20,
                      fontSize: 11,
                      fontWeight: 500,
                      bgcolor: 'rgba(99, 102, 241, 0.1)',
                      color: 'rgb(99, 102, 241)',
                    }}
                  />
                </Stack>

                <TableContainer>
                  <Table size="small" sx={{ minWidth: 500 }}>
                    <TableHead>
                      <TableRow>
                        <TableCell sx={{ fontWeight: 600, fontSize: 12 }}>Model</TableCell>
                        <TableCell sx={{ fontWeight: 600, fontSize: 12 }}>Status</TableCell>
                        <TableCell align="right" sx={{ fontWeight: 600, fontSize: 12 }}>
                          Score
                        </TableCell>
                        <TableCell align="right" sx={{ fontWeight: 600, fontSize: 12 }}>
                          Test Cases
                        </TableCell>
                      </TableRow>
                    </TableHead>
                    <TableBody>
                      {example.solutions.map((solution) => (
                        <TableRow
                          key={solution.model}
                          sx={{
                            '&:last-child td, &:last-child th': { border: 0 },
                            bgcolor: solution.passed ? 'rgba(34, 197, 94, 0.05)' : 'transparent',
                          }}
                        >
                          <TableCell sx={{ fontSize: 13, fontWeight: 500 }}>{solution.model}</TableCell>
                          <TableCell>
                            <Chip
                              label={solution.status}
                              size="small"
                              color={getStatusColor(solution.passed, solution.status)}
                              variant={solution.passed ? 'filled' : 'outlined'}
                              sx={{ height: 22, fontSize: 11 }}
                            />
                          </TableCell>
                          <TableCell align="right" sx={{ fontSize: 13, fontWeight: 500 }}>
                            {solution.score.toFixed(2)}
                          </TableCell>
                          <TableCell align="right" sx={{ fontSize: 13 }}>
                            <span style={{ color: solution.passedCases > 0 ? '#16a34a' : '#64748b' }}>
                              {solution.passedCases}
                            </span>
                            <span style={{ color: '#94a3b8' }}> / {solution.totalCases}</span>
                          </TableCell>
                        </TableRow>
                      ))}
                    </TableBody>
                  </Table>
                </TableContainer>

                <Box sx={{ mt: 2 }}>
                  <Button
                    variant="outlined"
                    size="small"
                    onClick={() => setSelectedProblem({ problem: example.problem, displayName: example.displayName })}
                    sx={{
                      textTransform: 'none',
                      borderColor: 'rgba(99, 102, 241, 0.5)',
                      color: 'rgb(99, 102, 241)',
                      '&:hover': {
                        borderColor: 'rgb(99, 102, 241)',
                        bgcolor: 'rgba(99, 102, 241, 0.05)',
                      }
                    }}
                  >
                    View Problem Statement & Codes
                  </Button>
                </Box>
              </Stack>

              {idx < examples.length - 1 && (
                <Box sx={{ height: 1, bgcolor: 'divider', mt: 3 }} />
              )}
            </Box>
          ))}
        </Stack>
      </Stack>

      <ProblemDetailModal
        open={selectedProblem !== null}
        onClose={() => setSelectedProblem(null)}
        problem={selectedProblem?.problem || ''}
        problemName={selectedProblem?.displayName || ''}
      />
    </Paper>
  );
};
