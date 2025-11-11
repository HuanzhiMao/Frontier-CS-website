import React from 'react';
import { Chip, Grid, Paper, Stack, Typography } from '@mui/material';

const pillars = [
  {
    title: 'Trustworthy scoring',
    body: 'Deterministic judging, reproducible runners, and diff-friendly artefacts keep LLM improvements verifiable.',
  },
  {
    title: 'Research-ready signals',
    body: 'Difficulty-balanced suites, seasonal static drops, and live telemetry enable fine-grained academic benchmarking.',
  },
  {
    title: 'Operator friendly',
    body: 'Stream submissions, triage regressions, and diff post-mortems without juggling dashboards or spreadsheets.',
  },
];

export const About: React.FC = () => {
  return (
    <Stack spacing={{ xs: 4, md: 5 }}>
      <Stack spacing={2}>
        <Chip
          label="Inside LiveCodeBench"
          color="navLink"
          sx={{ alignSelf: 'flex-start', fontWeight: 600, letterSpacing: '0.06em' }}
        />
        <Typography
          variant="h3"
          sx={{
            fontWeight: 700,
            fontSize: { xs: '1.9rem', md: '2.3rem' },
            letterSpacing: '-0.02em',
          }}
        >
          Purpose-built infrastructure for evaluating coding intelligence
        </Typography>
        <Typography variant="body1" sx={{ color: 'text.secondary', maxWidth: 720 }}>
          LiveCodeBench Pro combines live task ingestion with meticulously curated static suites so that researchers and
          applied teams can compare LLM compilers, reasoners, and agents on equal footing.
        </Typography>
      </Stack>

      <Grid container spacing={{ xs: 3, md: 4 }}>
        {pillars.map((pillar) => (
          <Grid item xs={12} md={4} key={pillar.title}>
            <Paper sx={{ p: { xs: 3, md: 4 }, borderRadius: 3, height: '100%' }}>
              <Stack spacing={1.5}>
                <Typography variant="h6" sx={{ fontWeight: 600 }}>
                  {pillar.title}
                </Typography>
                <Typography variant="body2" sx={{ color: 'text.secondary' }}>
                  {pillar.body}
                </Typography>
              </Stack>
            </Paper>
          </Grid>
        ))}
      </Grid>

      <Paper sx={{ p: { xs: 3, md: 4 }, borderRadius: 3 }}>
        <Stack spacing={1.5}>
          <Typography variant="h6" sx={{ fontWeight: 600 }}>
            Built by contest veterans, tuned for agent teams
          </Typography>
          <Typography variant="body2" sx={{ color: 'text.secondary' }}>
            From IOI medalists to Kaggle Grandmasters, our contributors obsess over precise competitive programming
            fidelity—so you can focus on advancing model capability.
          </Typography>
        </Stack>
      </Paper>
    </Stack>
  );
};
