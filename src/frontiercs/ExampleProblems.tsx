import React, { useState, useEffect } from 'react';
import {
  Alert,
  Box,
  Card,
  CardContent,
  CircularProgress,
  Grid,
  Paper,
  Stack,
  Typography,
} from '@mui/material';
import { useRequest } from 'ahooks';
import { dataService } from '../data';
import { ProblemDetailModal } from './ProblemDetailModal';

interface ProblemExample {
  problem: string;
  displayName: string;
  title: string;
  summary: string;
  scores: {
    maxScore: number;
    modelScores: { model: string; score: number }[];
  };
}

// Curated problem information
const CURATED_PROBLEMS: Record<string, { title: string; summary: string }> = {
  llm1: {
    title: 'Treasure Packing',
    summary: 'Optimize treasure selection with dual constraints on weight and volume to maximize total value.',
  },
  llm10: {
    title: 'Tree Distance Query',
    summary: 'Interactive problem requiring strategic queries to discover distances in a weighted tree structure.',
  },
  llm11: {
    title: 'Palindrome Path',
    summary: 'Navigate a grid maze where valid movement sequences must form palindromic patterns.',
  },
  llm101: {
    title: 'Circuit Discovery',
    summary: 'Identify hidden AND/OR logic gates through minimal strategic queries to the circuit.',
  },
  llm104: {
    title: 'Attendance Check',
    summary: 'Find the absent student using range queries despite dishonest responses from other students.',
  },
  llm106: {
    title: 'Bipartite Verification',
    summary: 'Determine if a hidden graph is bipartite with minimal edge subset queries.',
  },
};

export const ExampleProblems: React.FC = () => {
  const [selectedProblem, setSelectedProblem] = useState<{
    problem: string;
    displayName: string;
    modelScores: { model: string; score: number }[];
  } | null>(null);
  const [examples, setExamples] = useState<ProblemExample[]>([]);

  const { data: cpResults, loading, error } = useRequest(async () => {
    return await dataService.loadCPResults();
  });

  // Select 6 curated problems and compute their scores
  useEffect(() => {
    if (!cpResults) return;

    const { results } = cpResults;
    const curatedProblemNames = Object.keys(CURATED_PROBLEMS);

    const examplesData = curatedProblemNames.map((problemName) => {
      const curatedInfo = CURATED_PROBLEMS[problemName];

      // Get all results for this problem (shot 0 only for display)
      const problemResults = results.filter(
        (r: any) => r.problem === problemName && r.shot === 0
      );

      // Calculate model scores
      const modelScores = problemResults.map((r: any) => ({
        model: r.model.split(' ')[0], // Short name: "GPT-5-2025-08-07 (FC)" -> "GPT-5-2025-08-07"
        score: r.score || 0,
      }));

      // Find max score among all models
      const maxScore = Math.max(...modelScores.map(m => m.score), 0);

      return {
        problem: problemName,
        displayName: curatedInfo.title,
        title: curatedInfo.title,
        summary: curatedInfo.summary,
        scores: {
          maxScore,
          modelScores,
        },
      };
    });

    setExamples(examplesData);
  }, [cpResults]);

  if (loading || examples.length === 0) {
    return (
      <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', minHeight: 300 }}>
        <CircularProgress />
      </Box>
    );
  }

  if (error) {
    return <Alert severity="error">Failed to load example problems: {error.message}</Alert>;
  }

  // Color palette for cards
  const cardColors = [
    { gradient: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)', light: '#f3e8ff' },
    { gradient: 'linear-gradient(135deg, #f093fb 0%, #f5576c 100%)', light: '#fce7f3' },
    { gradient: 'linear-gradient(135deg, #4facfe 0%, #00f2fe 100%)', light: '#dbeafe' },
    { gradient: 'linear-gradient(135deg, #43e97b 0%, #38f9d7 100%)', light: '#d1fae5' },
    { gradient: 'linear-gradient(135deg, #fa709a 0%, #fee140 100%)', light: '#fef3c7' },
    { gradient: 'linear-gradient(135deg, #30cfd0 0%, #330867 100%)', light: '#e0e7ff' },
  ];

  return (
    <Paper
      sx={{
        p: { xs: 2, md: 3 },
        background: 'linear-gradient(135deg, #fdfbfb 0%, #ebedee 100%)',
      }}
    >
      <Stack spacing={3}>
        <Box
          sx={{
            background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
            borderRadius: 2,
            p: 2.5,
            color: 'white',
          }}
        >
          <Typography variant="h6" sx={{ fontWeight: 600, fontSize: '1.125rem' }}>
            Example Problems
          </Typography>
        </Box>

        <Grid container spacing={2.5}>
          {examples.map((example, index) => {
            const colorTheme = cardColors[index % cardColors.length];
            return (
              <Grid item xs={12} sm={6} md={4} key={example.problem}>
                <Card
                  sx={{
                    height: '100%',
                    cursor: 'pointer',
                    transition: 'all 0.3s ease',
                    border: '1px solid',
                    borderColor: 'divider',
                    position: 'relative',
                    overflow: 'hidden',
                    '&::before': {
                      content: '""',
                      position: 'absolute',
                      top: 0,
                      left: 0,
                      right: 0,
                      height: '4px',
                      background: colorTheme.gradient,
                    },
                    '&:hover': {
                      borderColor: 'primary.main',
                      boxShadow: '0 8px 24px rgba(0, 0, 0, 0.12)',
                      transform: 'translateY(-4px)',
                    },
                  }}
                  onClick={() =>
                    setSelectedProblem({
                      problem: example.problem,
                      displayName: example.title,
                      modelScores: example.scores.modelScores,
                    })
                  }
                >
                  <CardContent sx={{ pt: 2.5 }}>
                    <Stack spacing={1.5}>
                      <Box
                        sx={{
                          display: 'inline-block',
                          px: 1.5,
                          py: 0.5,
                          borderRadius: '12px',
                          background: colorTheme.light,
                          alignSelf: 'flex-start',
                          mb: 0.5,
                        }}
                      >
                        <Typography
                          variant="caption"
                          sx={{
                            fontWeight: 700,
                            fontSize: '0.7rem',
                            textTransform: 'uppercase',
                            letterSpacing: '0.05em',
                            background: colorTheme.gradient,
                            WebkitBackgroundClip: 'text',
                            WebkitTextFillColor: 'transparent',
                            backgroundClip: 'text',
                          }}
                        >
                          Problem {index + 1}
                        </Typography>
                      </Box>
                      <Typography
                        variant="h6"
                        sx={{
                          fontWeight: 700,
                          fontSize: '1.05rem',
                          lineHeight: 1.4,
                          color: '#1a202c',
                        }}
                      >
                        {example.title}
                      </Typography>
                      <Typography
                        variant="body2"
                        color="text.secondary"
                        sx={{
                          fontSize: '0.875rem',
                          lineHeight: 1.6,
                        }}
                      >
                        {example.summary}
                      </Typography>
                    </Stack>
                  </CardContent>
                </Card>
              </Grid>
            );
          })}
        </Grid>
      </Stack>

      <ProblemDetailModal
        open={selectedProblem !== null}
        onClose={() => setSelectedProblem(null)}
        problem={selectedProblem?.problem || ''}
        problemName={selectedProblem?.displayName || ''}
        modelScores={selectedProblem?.modelScores || []}
      />
    </Paper>
  );
};
