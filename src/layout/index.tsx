import React, { useCallback, useEffect, useMemo, useState } from 'react';
import { Box, Button, Container, Stack, Typography } from '@mui/material';
import { Outlet, useLocation, useNavigate } from 'react-router-dom';

import { defaultHero, findProjectByPath } from '../projects/config';
import { findBlogByPath } from '../blog';
import { OverviewFilter, OverviewFilterContext } from './OverviewFilterContext';

export const Layout: React.FC = () => {
  const navigate = useNavigate();
  const location = useLocation();
  const [overviewFilter, setOverviewFilter] = useState<OverviewFilter>('all');

  const activeProject = useMemo(() => findProjectByPath(location.pathname), [location.pathname]);
  const activeBlog = useMemo(() => findBlogByPath(location.pathname), [location.pathname]);

  useEffect(() => {
    const isOverviewRoute = location.pathname === '/' || location.pathname === '/projects';
    if (!isOverviewRoute && overviewFilter !== 'all') {
      setOverviewFilter('all');
    }
  }, [location.pathname, overviewFilter]);

  const currentHero = useMemo(() => {
    if (activeProject) {
      const matchedSection = activeProject.heroSections.find((section) => section.match(location.pathname));
      if (matchedSection) {
        return matchedSection;
      }
      return {
        title: activeProject.name,
        description: activeProject.heroSections[0]?.description ?? defaultHero.description,
      };
    }
    if (activeBlog) {
      return {
        title: activeBlog.title,
        description: activeBlog.heroDescription ?? activeBlog.summary,
      };
    }
    return defaultHero;
  }, [activeBlog, activeProject, location.pathname]);

  const projectUtilityLinks = useMemo(() => activeProject?.headerUtilityLinks ?? [], [activeProject]);
  const projectExternalLinks = useMemo(() => activeProject?.externalLinks ?? [], [activeProject]);
  const navigationLinks = activeProject?.navigationLinks ?? [];
  const externalInsertIndex = Math.min(1, Math.max(navigationLinks.length - 1, 0));

  const isUtilityActive = useCallback(
    (path: string) => {
      if (path === '/') {
        return location.pathname === '/' || location.pathname === '/projects';
      }
      return location.pathname === path || location.pathname.startsWith(`${path}/`);
    },
    [location.pathname],
  );

  const isProjectTabActive = useCallback(
    (path: string) => {
      if (!activeProject) return false;
      if (path === activeProject.overviewPath) {
        return (
          location.pathname === activeProject.basePath ||
          location.pathname === activeProject.overviewPath ||
          location.pathname === `${activeProject.overviewPath}/`
        );
      }
      return location.pathname === path || location.pathname.startsWith(`${path}/`);
    },
    [activeProject, location.pathname],
  );

  return (
    <Box sx={{ minHeight: '100vh', bgcolor: 'background.default', color: 'text.primary' }}>
      <Box
        component="header"
        sx={{
          borderBottom: '1px solid',
          borderColor: 'divider',
          bgcolor: 'background.default',
        }}
      >
        <Container maxWidth="lg" sx={{ py: 2 }}>
          <Stack direction="row" alignItems="center" justifyContent="space-between" spacing={3}>
            <Stack direction="row" alignItems="center" spacing={1.5}>
              <Button
                onClick={() => navigate('/')}
                variant="text"
                color="inherit"
                sx={{
                  fontSize: 15,
                  fontWeight: 600,
                  textTransform: 'none',
                  color:
                    location.pathname === '/' || location.pathname === '/projects'
                      ? 'text.primary'
                      : 'text.secondary',
                  '&:hover': {
                    backgroundColor: 'transparent',
                    color: 'primary.light',
                  },
                  ml: { xs: -0.5, md: -1 },
                  px: 0,
                }}
              >
                FrontierCS
              </Button>
            </Stack>

            <Stack direction="row" alignItems="center" spacing={1.5}>
              {projectUtilityLinks.map((link) => (
                <Button
                  key={link.path}
                  onClick={() => navigate(link.path)}
                  variant="text"
                  color="inherit"
                  sx={{
                    fontSize: 14,
                    fontWeight: isUtilityActive(link.path) ? 600 : 500,
                    color: isUtilityActive(link.path) ? 'primary.light' : 'text.secondary',
                    '&:hover': { backgroundColor: 'transparent', color: 'primary.light' },
                  }}
                >
                  {link.label}
                </Button>
              ))}
            </Stack>
          </Stack>
        </Container>
      </Box>

      <Box
        sx={{
          borderBottom: '1px solid',
          borderColor: 'divider',
          background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
        }}
      >
        <Container maxWidth="lg" sx={{ py: { xs: 4, md: 5 } }}>
          <Stack spacing={2}>
            <Stack spacing={2} alignItems="center" textAlign="center">
              <Typography
                variant="h2"
                sx={{
                  fontSize: { xs: '1.75rem', md: '2.2rem' },
                  fontWeight: 700,
                  color: 'white',
                }}
              >
                {currentHero.title}
              </Typography>
              <Typography
                variant="body1"
                sx={{
                  color: 'rgba(255, 255, 255, 0.95)',
                  maxWidth: {
                    xs: '100%',
                    md: activeBlog ? 600 : 800,
                  },
                  fontSize: { xs: '0.95rem', md: '1rem' },
                  lineHeight: 1.7,
                }}
              >
                {currentHero.description}
              </Typography>
            </Stack>

            <Stack direction="row" spacing={1} flexWrap="wrap" justifyContent="center">
              {navigationLinks.map((link, index) => {
                const active = isProjectTabActive(link.path);
                return (
                  <React.Fragment key={link.path}>
                    <Button
                      size="small"
                      variant={active ? 'contained' : 'text'}
                      color={active ? 'navPurple' : 'inherit'}
                      onClick={() => navigate(link.path)}
                      sx={{
                        fontWeight: active ? 700 : 500,
                        borderRadius: '999px',
                        px: 2.5,
                        py: 0.75,
                        ...(active
                          ? {
                              bgcolor: 'rgba(255, 255, 255, 0.95)',
                              color: '#667eea',
                              '&:hover': {
                                bgcolor: 'rgba(255, 255, 255, 1)',
                              },
                            }
                          : {
                              color: 'rgba(255, 255, 255, 0.8)',
                              '&:hover': {
                                bgcolor: 'rgba(255, 255, 255, 0.15)',
                                color: 'rgba(255, 255, 255, 1)',
                              },
                            }),
                      }}
                    >
                      {link.label}
                    </Button>
                    {index === externalInsertIndex &&
                      projectExternalLinks.map((externalLink) => (
                        <Button
                          key={externalLink.href}
                          size="small"
                          variant="text"
                          color="inherit"
                          component="a"
                          href={externalLink.href}
                          target="_blank"
                          rel="noopener"
                          sx={{
                            borderRadius: '999px',
                            px: 2.5,
                            py: 0.75,
                            fontSize: 13,
                            fontWeight: 500,
                            color: 'rgba(255, 255, 255, 0.8)',
                            '&:hover': {
                              bgcolor: 'rgba(255, 255, 255, 0.15)',
                              color: 'rgba(255, 255, 255, 1)',
                            },
                          }}
                        >
                          {externalLink.label}
                        </Button>
                      ))}
                  </React.Fragment>
                );
              })}
            </Stack>
          </Stack>
        </Container>
      </Box>

      <Container maxWidth="lg" sx={{ py: { xs: 3, md: 4 } }}>
        <OverviewFilterContext.Provider value={{ filter: overviewFilter, setFilter: setOverviewFilter }}>
          <Box component="main" sx={{ width: '100%' }}>
            <Outlet />
          </Box>
        </OverviewFilterContext.Provider>
      </Container>
    </Box>
  );
};
