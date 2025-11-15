import React, { useState, useMemo } from 'react';
import {
  Alert,
  Box,
  CircularProgress,
  Paper,
  Stack,
  Typography,
  ToggleButtonGroup,
  ToggleButton,
  Table,
  TableBody,
  TableCell,
  TableContainer,
  TableHead,
  TableRow,
  IconButton,
  Collapse,
} from '@mui/material';
import { MdExpandMore, MdChevronRight } from 'react-icons/md';
import { useRequest } from 'ahooks';
import { dataService } from '../data';

type ProblemType = 'research' | 'competitive';

export const UnifiedLeaderboard: React.FC = () => {
  const [problemType, setProblemType] = useState<ProblemType>('research');
  const [expandedRows, setExpandedRows] = useState<Set<string>>(new Set());
  const [expandAllRows, setExpandAllRows] = useState(false);
  const [showExtraColumns, setShowExtraColumns] = useState(false);

  // Load research leaderboard
  const { data: researchLeaderboard, loading: researchLoading, error: researchError } = useRequest(async () => {
    return await dataService.loadLeaderboard();
  });

  // Load competitive leaderboard
  const { data: cpLeaderboard, loading: cpLoading, error: cpError } = useRequest(async () => {
    return await dataService.loadCPLeaderboard();
  });

  // Load results data for problem counts
  const { data: researchResults } = useRequest(async () => {
    return await dataService.loadResults();
  });

  const { data: cpResults } = useRequest(async () => {
    return await dataService.loadCPResults();
  });

  // Filter models by selected problem type
  const tableRows = useMemo(() => {
    const leaderboard = problemType === 'research' ? researchLeaderboard : cpLeaderboard;
    if (!leaderboard) return [];

    return leaderboard
      .sort((a, b) => b.avgScore - a.avgScore)
      .map((model, index) => {
        return {
          rank: index + 1,
          id: `${model.provider}/${model.name}`,
          label: model.name,
          organization: model.provider ?? '—',
          license: (model as any).license ?? 'Unknown',
          avgScore: model.avgScore,
          categoryScores: (model as any).categoryScores || {},
        };
      });
  }, [problemType, researchLeaderboard, cpLeaderboard]);

  const toggleRow = (id: string) => {
    setExpandedRows(prev => {
      const newSet = new Set(prev);
      if (newSet.has(id)) {
        newSet.delete(id);
      } else {
        newSet.add(id);
      }
      return newSet;
    });
  };

  const toggleExpandAllRows = () => {
    if (expandAllRows) {
      setExpandedRows(new Set());
      setExpandAllRows(false);
    } else {
      const allRowIds = tableRows.map(row => row.id);
      setExpandedRows(new Set(allRowIds));
      setExpandAllRows(true);
    }
  };

  const toggleExtraColumns = () => {
    setShowExtraColumns(prev => !prev);
  };

  const loading = researchLoading || cpLoading;
  const error = researchError || cpError;

  if (loading) {
    return (
      <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', minHeight: 400 }}>
        <CircularProgress />
      </Box>
    );
  }

  if (error) {
    return <Alert severity="error">Failed to load leaderboard: {error.message}</Alert>;
  }

  // Helper function to get rank color/badge
  const getRankBadge = (rank: number) => {
    if (rank === 1) {
      return {
        background: 'linear-gradient(135deg, #FFD700 0%, #FFA500 100%)',
        color: '#fff',
        icon: '🥇',
      };
    } else if (rank === 2) {
      return {
        background: 'linear-gradient(135deg, #C0C0C0 0%, #808080 100%)',
        color: '#fff',
        icon: '🥈',
      };
    } else if (rank === 3) {
      return {
        background: 'linear-gradient(135deg, #CD7F32 0%, #8B4513 100%)',
        color: '#fff',
        icon: '🥉',
      };
    }
    return {
      background: '#f1f5f9',
      color: '#64748b',
      icon: '',
    };
  };

  return (
    <Stack spacing={3}>
      <Paper
        sx={{
          p: { xs: 2, md: 3 },
          background: 'linear-gradient(135deg, #fdfbfb 0%, #ebedee 100%)',
        }}
      >
        <Stack spacing={2.5}>
          <Box
            sx={{
              background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
              borderRadius: 2,
              p: 2.5,
              color: 'white',
            }}
          >
            <Stack
              direction={{ xs: 'column', md: 'row' }}
              spacing={2}
              alignItems={{ xs: 'flex-start', md: 'center' }}
              justifyContent="space-between"
            >
              <Typography variant="h6" sx={{ fontWeight: 600, fontSize: '1.125rem' }}>
                Frontier-CS Leaderboard
              </Typography>
              <ToggleButtonGroup
                value={problemType}
                exclusive
                onChange={(_, value) => {
                  if (value !== null) setProblemType(value);
                }}
                size="small"
                sx={{
                  bgcolor: 'rgba(255, 255, 255, 0.2)',
                  borderRadius: 2,
                  '& .MuiToggleButton-root': {
                    px: 2.5,
                    py: 0.75,
                    fontSize: 13,
                    textTransform: 'none',
                    color: 'rgba(255, 255, 255, 0.8)',
                    border: 'none',
                    fontWeight: 500,
                    '&:hover': {
                      bgcolor: 'rgba(255, 255, 255, 0.15)',
                    },
                    '&.Mui-selected': {
                      bgcolor: 'rgba(255, 255, 255, 0.95)',
                      color: '#667eea',
                      fontWeight: 700,
                      '&:hover': {
                        bgcolor: 'rgba(255, 255, 255, 1)',
                      },
                    },
                  },
                }}
              >
                <ToggleButton value="research">Research Problems</ToggleButton>
                <ToggleButton value="competitive">Algorithmic Problems</ToggleButton>
              </ToggleButtonGroup>
            </Stack>
          </Box>

          {tableRows.length === 0 && (
            <Typography variant="body2" sx={{ color: 'text.secondary' }}>
              No models available for this problem type.
            </Typography>
          )}

          {tableRows.length > 0 && (
            <TableContainer
              sx={{
                border: '1px solid',
                borderColor: 'divider',
                borderRadius: 2,
                overflowX: 'auto',
                overflowY: 'hidden',
              }}
            >
              <Table sx={{ minWidth: 650 }}>
                <TableHead>
                  <TableRow
                    sx={{
                      bgcolor: '#f8fafc',
                      borderBottom: '2px solid',
                      borderColor: 'divider',
                    }}
                  >
                    {problemType === 'research' && <TableCell sx={{ width: 50 }} />}
                    <TableCell
                      align="center"
                      sx={{
                        fontWeight: 700,
                        fontSize: '0.875rem',
                        color: '#475569',
                        textTransform: 'uppercase',
                        letterSpacing: '0.05em',
                        py: 2,
                        width: 80,
                      }}
                    >
                      Rank
                    </TableCell>
                    <TableCell
                      align="center"
                      sx={{
                        fontWeight: 700,
                        fontSize: '0.875rem',
                        color: '#475569',
                        textTransform: 'uppercase',
                        letterSpacing: '0.05em',
                        py: 2,
                      }}
                    >
                      Model
                    </TableCell>
                    <TableCell
                      align="center"
                      sx={{
                        fontWeight: 700,
                        fontSize: '0.875rem',
                        color: '#475569',
                        textTransform: 'uppercase',
                        letterSpacing: '0.05em',
                        py: 2,
                      }}
                    >
                      Organization
                    </TableCell>
                    <TableCell
                      align="center"
                      sx={{
                        fontWeight: 700,
                        fontSize: '0.875rem',
                        color: '#475569',
                        textTransform: 'uppercase',
                        letterSpacing: '0.05em',
                        py: 2,
                      }}
                    >
                      License
                    </TableCell>
                    <TableCell
                      align="center"
                      sx={{
                        fontWeight: 700,
                        fontSize: '0.875rem',
                        color: '#475569',
                        textTransform: 'uppercase',
                        letterSpacing: '0.05em',
                        py: 2,
                      }}
                    >
                      {problemType === 'research' ? (
                        'Score'
                      ) : (
                        showExtraColumns ? 'Score@1' : (
                          <Stack direction="row" alignItems="center" justifyContent="center" spacing={1}>
                            <span>Score@1</span>
                            <IconButton
                              size="small"
                              onClick={toggleExtraColumns}
                              sx={{
                                transition: 'transform 0.2s',
                                color: '#475569',
                                '&:hover': {
                                  bgcolor: 'rgba(0, 0, 0, 0.04)',
                                },
                              }}
                            >
                              <MdChevronRight size={16} />
                            </IconButton>
                          </Stack>
                        )
                      )}
                    </TableCell>
                    {problemType === 'competitive' && showExtraColumns && (
                      <>
                        <TableCell
                          align="center"
                          sx={{
                            fontWeight: 700,
                            fontSize: '0.875rem',
                            color: '#475569',
                            textTransform: 'uppercase',
                            letterSpacing: '0.05em',
                            py: 2,
                          }}
                        >
                          Avg@5
                        </TableCell>
                        <TableCell
                          align="center"
                          sx={{
                            fontWeight: 700,
                            fontSize: '0.875rem',
                            color: '#475569',
                            textTransform: 'uppercase',
                            letterSpacing: '0.05em',
                            py: 2,
                          }}
                        >
                          Score@5
                        </TableCell>
                        <TableCell
                          align="center"
                          sx={{
                            fontWeight: 700,
                            fontSize: '0.875rem',
                            color: '#475569',
                            textTransform: 'uppercase',
                            letterSpacing: '0.05em',
                            py: 2,
                          }}
                        >
                          Pass@1
                        </TableCell>
                        <TableCell
                          align="center"
                          sx={{
                            fontWeight: 700,
                            fontSize: '0.875rem',
                            color: '#475569',
                            textTransform: 'uppercase',
                            letterSpacing: '0.05em',
                            py: 2,
                          }}
                        >
                          <Stack direction="row" alignItems="center" justifyContent="center" spacing={1}>
                            <span>Pass@5</span>
                            <IconButton
                              size="small"
                              onClick={toggleExtraColumns}
                              sx={{
                                transition: 'transform 0.2s',
                                transform: 'rotate(180deg)',
                                color: '#475569',
                                '&:hover': {
                                  bgcolor: 'rgba(0, 0, 0, 0.04)',
                                },
                              }}
                            >
                              <MdChevronRight size={16} />
                            </IconButton>
                          </Stack>
                        </TableCell>
                      </>
                    )}
                  </TableRow>
                </TableHead>
                <TableBody>
                  {tableRows.map((row, index) => {
                    const isExpanded = expandedRows.has(row.id);
                    const categories = Object.keys(row.categoryScores);
                    const hasCategories = categories.length > 0 && problemType === 'research';

                    return (
                      <React.Fragment key={row.id}>
                        <TableRow
                          sx={{
                            bgcolor: index % 2 === 0 ? '#ffffff' : '#f9fafb',
                            '&:hover': {
                              bgcolor: '#f1f5f9',
                              transition: 'background-color 0.2s ease',
                            },
                            borderBottom: isExpanded ? 'none' : '1px solid',
                            borderColor: 'divider',
                          }}
                        >
                          {problemType === 'research' && (
                            <TableCell align="center" sx={{ py: 2, px: 2, width: 50 }}>
                              {hasCategories && (
                                <IconButton
                                  size="small"
                                  onClick={() => toggleRow(row.id)}
                                  sx={{
                                    transition: 'transform 0.2s',
                                    transform: isExpanded ? 'rotate(90deg)' : 'rotate(0deg)',
                                    color: '#475569',
                                    '&:hover': {
                                      bgcolor: 'rgba(0, 0, 0, 0.04)',
                                    },
                                  }}
                                >
                                  <MdChevronRight size={20} />
                                </IconButton>
                              )}
                            </TableCell>
                          )}
                          <TableCell
                            align="center"
                            sx={{
                              py: 2.5,
                              px: 3,
                            }}
                          >
                            <Box
                              sx={{
                                display: 'inline-flex',
                                alignItems: 'center',
                                justifyContent: 'center',
                                minWidth: 36,
                                height: 36,
                                borderRadius: '8px',
                                background: getRankBadge(row.rank).background,
                                color: getRankBadge(row.rank).color,
                                fontWeight: 700,
                                fontSize: '0.95rem',
                                boxShadow: row.rank <= 3 ? '0 2px 8px rgba(0,0,0,0.15)' : 'none',
                              }}
                            >
                              {getRankBadge(row.rank).icon || row.rank}
                            </Box>
                          </TableCell>
                          <TableCell align="center" sx={{ py: 2.5, px: 3 }}>
                            <Typography
                              variant="body1"
                              sx={{
                                fontWeight: 600,
                                fontSize: '0.95rem',
                                color: '#0f172a',
                              }}
                            >
                              {row.label}
                            </Typography>
                          </TableCell>
                          <TableCell align="center" sx={{ py: 2.5, px: 3 }}>
                            <Typography
                              variant="body2"
                              sx={{
                                color: '#64748b',
                                fontSize: '0.875rem',
                                fontWeight: 500,
                              }}
                            >
                              {row.organization}
                            </Typography>
                          </TableCell>
                          <TableCell align="center" sx={{ py: 2.5, px: 3 }}>
                            <Typography
                              variant="body2"
                              sx={{
                                color: '#64748b',
                                fontSize: '0.875rem',
                                fontWeight: 500,
                              }}
                            >
                              {row.license}
                            </Typography>
                          </TableCell>
                          <TableCell align="center" sx={{ py: 2.5, px: 3 }}>
                            <Box
                              sx={{
                                display: 'inline-block',
                                px: 2.5,
                                py: 1,
                                borderRadius: '12px',
                                background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
                                color: '#fff',
                                fontWeight: 700,
                                fontSize: '1.05rem',
                                boxShadow: '0 2px 8px rgba(0,0,0,0.12)',
                              }}
                            >
                              {row.avgScore.toFixed(2)}
                            </Box>
                          </TableCell>
                          {problemType === 'competitive' && showExtraColumns && (
                            <>
                              <TableCell align="center" sx={{ py: 2.5, px: 3 }}>
                                <Box
                                  sx={{
                                    display: 'inline-block',
                                    px: 2.5,
                                    py: 1,
                                    borderRadius: '12px',
                                    background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
                                    color: '#fff',
                                    fontWeight: 700,
                                    fontSize: '1.05rem',
                                    boxShadow: '0 2px 8px rgba(0,0,0,0.12)',
                                  }}
                                >
                                  {row.categoryScores['Avg@5']?.avgScore.toFixed(2) ?? '—'}
                                </Box>
                              </TableCell>
                              <TableCell align="center" sx={{ py: 2.5, px: 3 }}>
                                <Box
                                  sx={{
                                    display: 'inline-block',
                                    px: 2.5,
                                    py: 1,
                                    borderRadius: '12px',
                                    background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
                                    color: '#fff',
                                    fontWeight: 700,
                                    fontSize: '1.05rem',
                                    boxShadow: '0 2px 8px rgba(0,0,0,0.12)',
                                  }}
                                >
                                  {row.categoryScores['Score@5']?.avgScore.toFixed(2) ?? '—'}
                                </Box>
                              </TableCell>
                              <TableCell align="center" sx={{ py: 2.5, px: 3 }}>
                                <Box
                                  sx={{
                                    display: 'inline-block',
                                    px: 2.5,
                                    py: 1,
                                    borderRadius: '12px',
                                    background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
                                    color: '#fff',
                                    fontWeight: 700,
                                    fontSize: '1.05rem',
                                    boxShadow: '0 2px 8px rgba(0,0,0,0.12)',
                                  }}
                                >
                                  {row.categoryScores['Pass@1']?.avgScore.toFixed(2) ?? '—'}%
                                </Box>
                              </TableCell>
                              <TableCell align="center" sx={{ py: 2.5, px: 3 }}>
                                <Box
                                  sx={{
                                    display: 'inline-block',
                                    px: 2.5,
                                    py: 1,
                                    borderRadius: '12px',
                                    background: 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
                                    color: '#fff',
                                    fontWeight: 700,
                                    fontSize: '1.05rem',
                                    boxShadow: '0 2px 8px rgba(0,0,0,0.12)',
                                  }}
                                >
                                  {row.categoryScores['Pass@5']?.avgScore.toFixed(2) ?? '—'}%
                                </Box>
                              </TableCell>
                            </>
                          )}
                        </TableRow>

                        {hasCategories && (
                          <TableRow>
                            <TableCell
                              sx={{
                                py: 0,
                                px: 0,
                                borderBottom: '1px solid',
                                borderColor: 'divider',
                              }}
                              colSpan={6}
                            >
                              <Collapse in={isExpanded} timeout="auto" unmountOnExit>
                                <Box sx={{ bgcolor: '#fafbfc', py: 2, px: 6 }}>
                                  <Stack spacing={1.5}>
                                    {categories.map((category, idx) => {
                                      const catData = row.categoryScores[category];
                                      // Cycle through gradient colors for categories
                                      // Order: OS, HPC, AI, DB
                                      const gradients = [
                                        'linear-gradient(135deg, #ff6b6b 0%, #ee5a6f 100%)', // OS - Red
                                        'linear-gradient(135deg, #4facfe 0%, #00f2fe 100%)', // HPC - Blue
                                        'linear-gradient(135deg, #43e97b 0%, #38f9d7 100%)', // AI - Green
                                        'linear-gradient(135deg, #fa709a 0%, #fee140 100%)', // DB - Pink/Yellow
                                      ];
                                      return (
                                        <Stack
                                          key={category}
                                          direction="row"
                                          justifyContent="space-between"
                                          alignItems="center"
                                          sx={{
                                            py: 1.5,
                                            px: 2.5,
                                            bgcolor: '#ffffff',
                                            borderRadius: 2,
                                            border: '1px solid',
                                            borderColor: 'divider',
                                            boxShadow: '0 1px 3px rgba(0,0,0,0.05)',
                                          }}
                                        >
                                          <Typography
                                            variant="body2"
                                            sx={{
                                              fontWeight: 600,
                                              color: '#334155',
                                              fontSize: '0.875rem',
                                            }}
                                          >
                                            {category}
                                          </Typography>
                                          <Box
                                            sx={{
                                              px: 2,
                                              py: 0.75,
                                              borderRadius: '8px',
                                              background: gradients[idx % gradients.length],
                                              color: '#fff',
                                              fontWeight: 700,
                                              fontSize: '0.875rem',
                                              boxShadow: '0 2px 6px rgba(0,0,0,0.1)',
                                            }}
                                          >
                                            {catData.avgScore.toFixed(2)}
                                          </Box>
                                        </Stack>
                                      );
                                    })}
                                  </Stack>
                                </Box>
                              </Collapse>
                            </TableCell>
                          </TableRow>
                        )}
                      </React.Fragment>
                    );
                  })}
                </TableBody>
              </Table>
            </TableContainer>
          )}
        </Stack>
      </Paper>
    </Stack>
  );
};
