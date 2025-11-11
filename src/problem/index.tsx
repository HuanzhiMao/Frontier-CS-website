import React from 'react';
import { Chip, Grid, Paper, Stack, Typography } from '@mui/material';

const tracks = [
  {
    name: 'Static 2025 H1',
    description: 'Bi-annual curated mix of 180 tasks with deterministic replay runners.',
    makeup: 'Difficulty mix • hard 30% · medium 45% · easy 25%',
  },
  {
    name: 'Adaptive Live Stream',
    description: 'Continuously ingested judged submissions sourced from partner contests and user uploads.',
    makeup: 'Rotating problem set refreshed hourly with anti-leak heuristics.',
  },
  {
    name: 'Systems & Integration',
    description: 'Focuses on multi-file builds, toolchain access, and system-call constrained challenges.',
    makeup: 'Targets on-call readiness for production agent runners.',
  },
];

export const Problems: React.FC = () => {
  return (
    <Stack spacing={{ xs: 4, md: 5 }}>
      <Stack spacing={2}>
        <Chip
          label="Problem collections"
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
          Challenge sets that mirror the pace of real competitions
        </Typography>
        <Typography variant="body1" sx={{ color: 'text.secondary', maxWidth: 720 }}>
          Blend static quarterly drops with continuously refreshed live pools to stress-test reasoning, compilation, and
          optimisation ability across difficulty tiers.
        </Typography>
      </Stack>

      <Grid container spacing={{ xs: 3, md: 4 }}>
        {tracks.map((track) => (
          <Grid item xs={12} md={4} key={track.name}>
            <Paper sx={{ p: { xs: 3, md: 4 }, borderRadius: 3, height: '100%' }}>
              <Stack spacing={1.5}>
                <Typography variant="h6" sx={{ fontWeight: 600 }}>
                  {track.name}
                </Typography>
                <Typography variant="body2" sx={{ color: 'text.secondary' }}>
                  {track.description}
                </Typography>
                <Typography variant="caption" sx={{ color: 'text.secondary', textTransform: 'uppercase' }}>
                  {track.makeup}
                </Typography>
              </Stack>
            </Paper>
          </Grid>
        ))}
      </Grid>

      <Paper sx={{ p: { xs: 3, md: 4 }, borderRadius: 3 }}>
        <Stack spacing={1.5}>
          <Typography variant="h6" sx={{ fontWeight: 600 }}>
            Want to contribute challenges?
          </Typography>
          <Typography variant="body2" sx={{ color: 'text.secondary' }}>
            We partner with universities, contest writers, and product teams to grow the pool while maintaining freshness
            and fairness. Reach out through GitHub issues to collaborate.
          </Typography>
        </Stack>
      </Paper>
    </Stack>
  );
};
