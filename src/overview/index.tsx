import React from 'react';
import { Box, ButtonBase, Stack, Typography } from '@mui/material';
import { Link as RouterLink, useLocation } from 'react-router-dom';

import { buildOverviewEntries, OverviewEntry } from './entries';
import { UnifiedLeaderboard, ExampleProblems } from '../frontiercs';

export const ProjectsOverview: React.FC = () => {
  const overviewEntries: OverviewEntry[] = buildOverviewEntries();
  const location = useLocation();

  React.useEffect(() => {
    const hash = location.hash;
    if (!hash) return;
    const targetId = hash.slice(1);
    if (!targetId) return;
    const anchor = document.getElementById(targetId);
    if (anchor) {
      anchor.scrollIntoView({ behavior: 'smooth', block: 'start' });
    }
  }, [location.hash]);

  const cardSx = (index: number, length: number) => ({
    width: '100%',
    display: 'flex',
    flexDirection: { xs: 'column', md: 'row' },
    alignItems: { xs: 'flex-start', md: 'center' },
    gap: { xs: 1.5, md: 3 },
    textAlign: 'left',
    textDecoration: 'none',
    borderBottom: index === length - 1 ? 'none' : '1px solid',
    borderColor: 'divider',
    py: { xs: 2.5, md: 3 },
    px: 0,
    color: 'inherit',
    position: 'relative',
    borderRadius: 0,
    transition: 'background-color 180ms ease, color 180ms ease',
    pl: 2,
    '&::before': {
      content: '""',
      position: 'absolute',
      left: 0,
      top: '18%',
      bottom: '18%',
      width: 4,
      borderRadius: 999,
      background: 'linear-gradient(180deg, rgba(163, 163, 255, 0.4), rgba(129, 140, 248, 1))',
      opacity: 0,
      transition: 'opacity 200ms ease',
    },
    '&:hover': {
      backgroundColor: 'transparent',
      textDecoration: 'none',
      '& .project-title': {
        color: 'primary.light',
      },
      '&::before': {
        opacity: 1,
      },
    },
    '&:focus-visible': {
      outline: 'none',
      boxShadow: '0 0 0 2px rgba(120, 119, 198, 0.4)',
      textDecoration: 'none',
      '&::before': {
        opacity: 1,
      },
    },
  });

  return (
    <Stack spacing={{ xs: 2.5, md: 3 }} sx={{ pb: { xs: 4, md: 6 } }}>
      <UnifiedLeaderboard />

      <ExampleProblems />

      <Box sx={{ height: { xs: 12, md: 20 } }} />

      {overviewEntries.length > 0 && (
        <Stack spacing={{ xs: 2.5, md: 3 }}>
          <Stack spacing={0}>
            {overviewEntries.map((entry, index) => (
              <ButtonBase
                key={entry.key}
                component={RouterLink}
                to={entry.path}
                id={`overview-${entry.key}`}
                sx={cardSx(index, overviewEntries.length)}
              >
                <Stack
                  spacing={0.5}
                  sx={{
                    minWidth: { md: 200 },
                    alignSelf: { md: 'stretch' },
                    justifyContent: 'center',
                  }}
                >
                  <Typography
                    variant="subtitle1"
                    sx={{ fontWeight: 500, fontSize: { xs: '0.95rem', md: '1.05rem' } }}
                  >
                    {entry.label}
                  </Typography>
                  <Typography
                    variant="body1"
                    sx={{ color: 'text.secondary', fontSize: { xs: '0.9rem', md: '1rem' } }}
                  >
                    {entry.subtitle}
                  </Typography>
                </Stack>

                <Stack spacing={1} sx={{ flex: 1, alignSelf: 'stretch', justifyContent: 'center' }}>
                  <Typography
                    className="project-title"
                    variant="h4"
                    sx={{
                      fontSize: { xs: '1.4rem', md: '1.75rem' },
                      fontWeight: 400,
                      letterSpacing: 0.2,
                      color: 'text.primary',
                      transition: 'color 180ms ease',
                    }}
                  >
                    {entry.title}
                  </Typography>
                  <Typography variant="body1" sx={{ color: 'text.secondary', maxWidth: 720 }}>
                    {entry.description}
                  </Typography>
                </Stack>
              </ButtonBase>
            ))}
          </Stack>
        </Stack>
      )}
    </Stack>
  );
};
