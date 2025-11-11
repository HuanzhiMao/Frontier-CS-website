import React from 'react';
import { Box, ButtonBase, Stack, Typography } from '@mui/material';
import { Link as RouterLink, useLocation } from 'react-router-dom';
import { buildQuickNavigationItems } from '../overview/entries';
import { useOverviewFilter } from './OverviewFilterContext';

export const QuickNavigation: React.FC = () => {
  const location = useLocation();
  const { filter } = useOverviewFilter();

  const allItems = React.useMemo(() => buildQuickNavigationItems(), []);
  const shouldFilter = location.pathname === '/' || location.pathname === '/projects';
  const items = React.useMemo(() => {
    if (!shouldFilter || filter === 'all') {
      return allItems;
    }
    return allItems.filter((item) => item.label === filter);
  }, [allItems, filter, shouldFilter]);

  const handleNavClick = React.useCallback(() => {
    window.scrollTo({ top: 0, behavior: 'smooth' });
  }, []);

  return (
    <Stack
      component="nav"
      spacing={1}
      sx={{
        width: { xs: '100%', md: 240 },
        minWidth: { md: 240 },
        position: { md: 'sticky' },
        top: { md: 128 },
        alignSelf: { md: 'flex-start' },
        borderLeft: { md: '1px solid' },
        borderColor: { md: 'divider' },
        pl: { md: 2.5 },
        flexShrink: { md: 0 },
        flexGrow: 0,
      }}
    >
      <Typography
        variant="subtitle2"
        sx={{ color: 'text.secondary', fontWeight: 500, fontSize: '0.85rem', mb: 1 }}
      >
        Latest Advancements
      </Typography>
      <Stack component="ul" spacing={0.75} sx={{ listStyle: 'none', m: 0, p: 0 }}>
        {items.map((item) => (
          <Box component="li" key={item.key}>
            <ButtonBase
              component={RouterLink}
              to={item.path}
              onClick={handleNavClick}
              sx={{
                width: '100%',
                justifyContent: 'flex-start',
                px: 1,
                py: 0.75,
                borderRadius: 1,
                color: 'text.primary',
                textAlign: 'left',
                fontSize: '0.95rem',
                transition: 'background-color 160ms ease, color 160ms ease',
                '&:hover': {
                  backgroundColor: 'rgba(129, 140, 248, 0.08)',
                  color: 'primary.light',
                  textDecoration: 'none',
                },
                '&:focus-visible': {
                  outline: 'none',
                  boxShadow: '0 0 0 2px rgba(120, 119, 198, 0.4)',
                },
              }}
            >
              {item.title}
            </ButtonBase>
          </Box>
        ))}
      </Stack>

      <Box sx={{ pt: 2 }}>
        <Typography
          variant="subtitle2"
          sx={{ color: 'text.secondary', fontWeight: 500, fontSize: '0.85rem', mb: 1 }}
        >
          Quick Links
        </Typography>
        <ButtonBase
          component={RouterLink}
          to="/"
          onClick={handleNavClick}
          sx={{
            width: '100%',
            justifyContent: 'flex-start',
            px: 1,
            py: 0.75,
            borderRadius: 1,
            color: 'text.primary',
            textAlign: 'left',
            fontSize: '0.95rem',
            transition: 'background-color 160ms ease, color 160ms ease',
            '&:hover': {
              backgroundColor: 'rgba(129, 140, 248, 0.08)',
              color: 'primary.light',
              textDecoration: 'none',
            },
            '&:focus-visible': {
              outline: 'none',
              boxShadow: '0 0 0 2px rgba(120, 119, 198, 0.4)',
            },
          }}
        >
          FrontierCS
        </ButtonBase>
      </Box>
    </Stack>
  );
};
