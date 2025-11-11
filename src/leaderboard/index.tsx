import { Alert, CircularProgress, Modal, Paper, Typography, Button, Box, TextField, Accordion, AccordionSummary, AccordionDetails, Chip, Stack } from "@mui/material"
import { DataGrid, GridColDef } from '@mui/x-data-grid';
import { listLlmLeaderboardLlmGet, Llm, BenchmarkMode, LlmByDifficulty,
	listLlmByDifficultyLeaderboardLlmDifficultyGet } from "../api";
import { useRequest } from "ahooks";
// import styled from "@emotion/styled";
import { LLMSubmissionsViewer } from "./llmsubmissions";
import { useState, useEffect, useMemo } from "react";
import { createTheme, PaletteColorOptions, PaletteColor, Palette, Theme } from '@mui/material/styles';

import Tab from '@mui/material/Tab';
// import LinearProgress from '@mui/material/LinearProgress';
import TabContext from '@mui/lab/TabContext';
import TabList from '@mui/lab/TabList';
import TabPanel from '@mui/lab/TabPanel';
import { LineChart } from '@mui/x-charts/LineChart';
import { CustomAxisTooltip } from './AxisTooltip';
import { FaChevronDown, FaEye, FaEyeSlash, FaSearch } from 'react-icons/fa';
import InputAdornment from '@mui/material/InputAdornment';

// Type alias to mark Palette and Theme as used for module augmentation context
// Exporting it marks it as used by the TypeScript compiler.
export type _ThemeAugmentation = Palette & Theme;

// Local UI type to support additional static variants not in API enum
type UiBenchmarkMode = BenchmarkMode | 'static_2024q4' | 'static_2025q1' | 'static_2025q2' | 'static_2025q3';

// Augment the MUI theme types to include the custom navLink color and MuiDataGrid component overrides
declare module '@mui/material/styles' {
  interface PaletteOptions {
    navLink?: PaletteColorOptions;
    info?: PaletteColorOptions;
    arxivRed?: PaletteColorOptions;
    navPurple?: PaletteColorOptions;
    navOrange?: PaletteColorOptions;
    navTeal?: PaletteColorOptions;
  }
  interface Palette {
    navLink: PaletteColor;
    info: PaletteColor;
    arxivRed: PaletteColor;
    navPurple: PaletteColor;
    navOrange: PaletteColor;
    navTeal: PaletteColor;
  }

  // Augment Components interface for MuiDataGrid
  interface Components<Theme = unknown> {
    MuiDataGrid?: {
      defaultProps?: import('@mui/x-data-grid').DataGridProps; // Keep for completeness
      // More direct styleOverrides definition for MuiDataGrid
      styleOverrides?: Record<string, any> | ((props: { theme: Theme } & Record<string, any>) => Record<string, any>) ;
      // Similarly for variants if needed, though we are not using them here
      // variants?: any[]; 
    };
  }
}

declare module '@mui/material/Chip' {
  interface ChipPropsColorOverrides {
    navLink: true;
  }
}

declare module '@mui/material/Button' {
  interface ButtonPropsColorOverrides {
    navLink: true;
    arxivRed: true;
    navPurple: true;
    navOrange: true;
    navTeal: true;
  }
}



export const geekTheme = createTheme({
  palette: {
    mode: 'light',
    primary: {
      main: '#111827',
      light: '#1f2937',
      dark: '#0b1120',
      contrastText: '#f8fafc',
    },
    secondary: {
      main: '#334155',
      light: '#475569',
      dark: '#1e293b',
      contrastText: '#f8fafc',
    },
    background: {
      default: '#ffffff',
      paper: '#f9fafb',
    },
    text: {
      primary: '#0f172a',
      secondary: '#475569',
      disabled: '#94a3b8',
    },
    divider: 'rgba(15, 23, 42, 0.12)',
    chartColors: [
      '#1d4ed8',
      '#2563eb',
      '#3b82f6',
      '#6366f1',
      '#4338ca',
      '#0891b2',
      '#0ea5e9',
      '#15803d',
    ],
    action: {
      active: '#111827',
      hover: 'rgba(15, 23, 42, 0.08)',
      selected: 'rgba(15, 23, 42, 0.14)',
      disabled: 'rgba(148, 163, 184, 0.6)',
      disabledBackground: 'rgba(148, 163, 184, 0.12)',
    },
    success: {
      main: '#15803d',
    },
    error: {
      main: '#dc2626',
    },
    info: {
      main: '#2563eb',
      dark: '#1d4ed8',
      contrastText: '#f8fafc',
    },
    arxivRed: {
      main: '#dc2626',
      dark: '#b91c1c',
      contrastText: '#ffffff',
    },
    navLink: {
      main: '#e2e8f0',
      dark: '#cbd5f5',
      contrastText: '#1e293b',
    },
    navPurple: {
      main: '#4338ca',
      dark: '#312e81',
      contrastText: '#eef2ff',
    },
    navOrange: {
      main: '#f97316',
      dark: '#ea580c',
      contrastText: '#ffffff',
    },
    navTeal: {
      main: '#0f766e',
      dark: '#115e59',
      contrastText: '#ffffff',
    },
  },
  typography: {
    fontFamily: '"Inter", "Segoe UI", -apple-system, BlinkMacSystemFont, "Helvetica Neue", sans-serif',
    h1: {
      color: '#0f172a',
      fontWeight: 700,
      fontSize: '2.6rem',
      letterSpacing: '-0.02em',
    },
    h2: {
      color: '#0f172a',
      fontWeight: 600,
      fontSize: '2rem',
      letterSpacing: '-0.015em',
    },
    h5: {
      color: '#0f172a',
      fontWeight: 600,
    },
    body1: {
      color: '#0f172a',
    },
    body2: {
      color: '#475569',
    },
    button: {
      fontWeight: 600,
      letterSpacing: '0.01em',
    },
  },
  components: {
    MuiCssBaseline: {
      styleOverrides: {
        body: {
          backgroundColor: '#ffffff',
          color: '#0f172a',
        },
        a: {
          color: '#0f172a',
        },
      },
    },
    MuiPaper: {
      styleOverrides: {
        root: {
          backgroundColor: '#ffffff',
          border: '1px solid rgba(15, 23, 42, 0.08)',
          boxShadow: 'none',
          borderRadius: 12,
        },
      },
    },
    MuiButton: {
      styleOverrides: {
        root: ({ ownerState, theme }) => {
          const styles: Record<string, any> = {
            textTransform: 'none',
            borderRadius: 6,
            fontWeight: 600,
          };

          if (ownerState.variant === 'contained') {
            const paletteKey = ownerState.color || 'primary';
            const paletteColor = theme.palette[paletteKey as keyof Palette] as PaletteColor | undefined;
            styles.boxShadow = 'none';
            styles['&:hover'] = {
              boxShadow: 'none',
              backgroundColor: paletteColor?.dark || '#0b1120',
            };
          }

          if (ownerState.variant === 'outlined') {
            styles.borderWidth = 1;
            styles['&:hover'] = {
              borderColor: theme.palette.primary.light,
              backgroundColor: 'rgba(15, 23, 42, 0.08)',
            };
          }

          return styles;
        },
      },
    },
    MuiChip: {
      styleOverrides: {
        root: {
          borderRadius: 6,
          backgroundColor: '#e2e8f0',
          color: '#1e293b',
        },
      },
    },
    MuiTextField: {
      styleOverrides: {
        root: {
          '& .MuiOutlinedInput-root': {
            borderRadius: 6,
            '& fieldset': {
              borderColor: '#cbd5f5',
            },
            '&:hover fieldset': {
              borderColor: '#94a3b8',
            },
            '&.Mui-focused fieldset': {
              borderColor: '#2563eb',
              boxShadow: '0 0 0 1px rgba(37, 99, 235, 0.2)',
            },
          },
        },
      },
    },
    MuiDataGrid: {
      styleOverrides: {
        root: {
          border: '1px solid rgba(15, 23, 42, 0.12)',
          borderRadius: 12,
          backgroundColor: '#ffffff',
          color: '#0f172a',
          '& .MuiDataGrid-columnHeaders': {
            backgroundColor: '#f1f5f9',
            borderBottom: '1px solid rgba(15, 23, 42, 0.12)',
            fontWeight: 600,
          },
          '& .MuiDataGrid-cell': {
            borderBottom: '1px solid rgba(148, 163, 184, 0.2)',
            display: 'flex',
            alignItems: 'center',
            py: '0 !important',
          },
          '& .MuiDataGrid-row:hover': {
            backgroundColor: 'rgba(15, 23, 42, 0.04)',
          },
          '& .MuiDataGrid-row': {
            alignItems: 'center',
          },
          '& .MuiDataGrid-row.Mui-selected': {
            backgroundColor: 'rgba(15, 23, 42, 0.08)',
          },
        },
        footerContainer: {
          borderTop: '1px solid rgba(15, 23, 42, 0.12)',
        },
      },
    },
    MuiTab: {
      styleOverrides: {
        root: {
          textTransform: 'none',
          fontWeight: 600,
          minHeight: 44,
        },
      },
    },
    MuiTabs: {
      styleOverrides: {
        indicator: {
          height: 2,
          backgroundColor: '#111827',
        },
      },
    },
  },
});

const get_llm_show_name = (x: Llm): string => {
	return ( x.display_name !== null && x.display_name !== undefined )? x.display_name: x.name;
}

// Helper function to get problem set name based on benchmark mode
const getProblemSetName = (mode: UiBenchmarkMode): string | null => {
  if (mode === 'static' || mode === 'static_2024q4') return 'quater_2024_10_12';
  if (mode === 'static_2025q1') return 'quater_2025_1_3';
  if (mode === 'static_2025q2') return 'quater_2025_4_6';
  if (mode === 'static_2025q3') return 'quater_2025_7_9';
  return null;
};

// Helper function to get the actual benchmark mode for API calls
const getApiBenchmarkMode = (mode: UiBenchmarkMode): BenchmarkMode => {
  // Static datasets should use 'static' as the benchmark mode
  if (mode === 'static_2024q4' || mode === 'static_2025q1' || mode === 'static_2025q2' || mode === 'static_2025q3') {
    return 'static';
  }
  return mode;
};

// Combined submissions viewer component that shows both difficulty and rating submissions
const CombinedSubmissionsViewer: React.FC<{
  name: string; 
  provider: string; 
  benchMode: BenchmarkMode;
  originalBenchMode: UiBenchmarkMode;
}> = ({name, provider, benchMode, originalBenchMode}) => {
  const [submissionTab, setSubmissionTab] = useState('difficulty');
  const [difficultySubTab, setDifficultySubTab] = useState('hard');

  const handleSubmissionTabChange = (_event: React.SyntheticEvent, newValue: string) => {
    setSubmissionTab(newValue);
  };

  const handleDifficultySubTabChange = (_event: React.SyntheticEvent, newValue: string) => {
    setDifficultySubTab(newValue);
  };

  // Get the problem set name for the current benchmark mode using the original tab value
  const problemSetName = useMemo(() => {
    return getProblemSetName(originalBenchMode);
  }, [originalBenchMode]);

  return (
    <Box sx={{ width: '100%' }}>
      <Typography variant="h4" gutterBottom>
        Submissions for {name} ({provider})
      </Typography>
      
      <TabContext value={submissionTab}>
        <Box sx={{ borderBottom: 1, borderColor: 'divider', mb: 2 }}>
          <TabList onChange={handleSubmissionTabChange} aria-label="Submission type tabs">
            <Tab label="Difficulty-based Problems" value="difficulty" />
            <Tab label="Rating-based Problems" value="rating" />
          </TabList>
        </Box>
        
        <TabPanel value="difficulty" sx={{ p: 0 }}>
          <TabContext value={difficultySubTab}>
            <Box sx={{ borderBottom: 1, borderColor: 'divider', mb: 2 }}>
              <TabList onChange={handleDifficultySubTabChange} aria-label="Difficulty tabs">
                <Tab label="Hard" value="hard" />
                <Tab label="Medium" value="medium" />
                <Tab label="Easy" value="easy" />
              </TabList>
            </Box>
            
            <TabPanel value="hard" sx={{ p: 0 }}>
              <Box sx={{ 
                maxHeight: '60vh', 
                overflow: 'auto',
                '& > div': {
                  position: 'relative !important',
                  top: 'auto !important',
                  left: 'auto !important',
                  transform: 'none !important',
                  bgcolor: 'transparent !important',
                  border: 'none !important',
                  boxShadow: 'none !important',
                  p: '0 !important',
                  width: '100% !important',
                  height: 'auto !important'
                },
                '& h4': {
                  display: 'none !important'
                }
              }}>
                <LLMSubmissionsViewer 
                  name={name} 
                  provider={provider} 
                  difficulty='hard' 
                  bench_mode={benchMode} 
                  problemSetName={problemSetName}
                />
              </Box>
            </TabPanel>

            <TabPanel value="medium" sx={{ p: 0 }}>
              <Box sx={{ 
                maxHeight: '60vh', 
                overflow: 'auto',
                '& > div': {
                  position: 'relative !important',
                  top: 'auto !important',
                  left: 'auto !important',
                  transform: 'none !important',
                  bgcolor: 'transparent !important',
                  border: 'none !important',
                  boxShadow: 'none !important',
                  p: '0 !important',
                  width: '100% !important',
                  height: 'auto !important'
                },
                '& h4': {
                  display: 'none !important'
                }
              }}>
                <LLMSubmissionsViewer 
                  name={name} 
                  provider={provider} 
                  difficulty='medium' 
                  bench_mode={benchMode} 
                  problemSetName={problemSetName}
                />
              </Box>
            </TabPanel>

            <TabPanel value="easy" sx={{ p: 0 }}>
              <Box sx={{ 
                maxHeight: '60vh', 
                overflow: 'auto',
                '& > div': {
                  position: 'relative !important',
                  top: 'auto !important',
                  left: 'auto !important',
                  transform: 'none !important',
                  bgcolor: 'transparent !important',
                  border: 'none !important',
                  boxShadow: 'none !important',
                  p: '0 !important',
                  width: '100% !important',
                  height: 'auto !important'
                },
                '& h4': {
                  display: 'none !important'
                }
              }}>
                <LLMSubmissionsViewer 
                  name={name} 
                  provider={provider} 
                  difficulty='easy' 
                  bench_mode={benchMode} 
                  problemSetName={problemSetName}
                />
              </Box>
            </TabPanel>
          </TabContext>
        </TabPanel>
        
        <TabPanel value="rating" sx={{ p: 0 }}>
          <Box sx={{ 
            maxHeight: '60vh', 
            overflow: 'auto',
            '& > div': {
              position: 'relative !important',
              top: 'auto !important',
              left: 'auto !important',
              transform: 'none !important',
              bgcolor: 'transparent !important',
              border: 'none !important',
              boxShadow: 'none !important',
              p: '0 !important',
              width: '100% !important',
              height: 'auto !important'
            },
            '& h4': {
              display: 'none !important'
            }
          }}>
            <LLMSubmissionsViewer 
              name={name} 
              provider={provider} 
              difficulty='all' 
              bench_mode='rating' 
              problemSetName={null}
            />
          </Box>
        </TabPanel>
      </TabContext>
    </Box>
  );
};

// Combined difficulty columns for the new table
const renderPassCell = (color: string) => (params: any) => {
  const raw = params.value ?? 0;
  const percentage = Math.max(0, Math.min(100, raw * 100));
  const formattedValue = Number.parseFloat(percentage.toFixed(1));
  const formattedPercent = formattedValue.toFixed(1);
  return (
    <Stack
      className="progress-cell"
      direction="row"
      alignItems="center"
      justifyContent="flex-start"
      spacing={2}
      sx={{ width: '100%', height: '100%', px: { xs: 2, sm: 3 }, py: 0 }}
    >
      <Box sx={{ flex: 1, display: 'flex', alignItems: 'center', gap: 1.5 }}>
        <Typography
          variant="body2"
          sx={{
            color: '#0f172a',
            fontWeight: 600,
            fontSize: 14,
            flexShrink: 0,
          }}
        >
          {formattedPercent}%
        </Typography>
        <Box
          sx={{
            width: '100%',
            maxWidth: 320,
            bgcolor: 'rgba(15, 23, 42, 0.12)',
            borderRadius: 999,
            overflow: 'hidden',
            height: 8,
          }}
        >
          <Box sx={{ width: `${percentage}%`, bgcolor: color, height: '100%', borderRadius: 999 }} />
        </Box>
      </Box>
    </Stack>
  );
};

const parseRatingValue = (value: unknown): number | null => {
  if (value === null || value === undefined) {
    return null;
  }
  if (typeof value === 'number') {
    return Number.isFinite(value) ? value : null;
  }
  if (typeof value === 'string' && value.trim() !== '') {
    const parsed = Number.parseFloat(value);
    return Number.isFinite(parsed) ? parsed : null;
  }
  return null;
};

const computeRating = (model: { rating?: number | string | null; ratingEvent?: Array<{ rating?: number | string | null }> }) => {
  const direct = parseRatingValue(model.rating);
  if (direct !== null) {
    return direct;
  }

  const events = model.ratingEvent ?? [];
  for (let idx = events.length - 1; idx >= 0; idx -= 1) {
    const eventRating = parseRatingValue(events[idx]?.rating);
    if (eventRating !== null) {
      return eventRating;
    }
  }

  return 0;
};

type CombinedLeaderboardRow = Llm & {
  hardPassrate: number;
  mediumPassrate: number;
  easyPassrate: number;
  rating: number;
  rawRating?: number | string | null;
  hardValidrate: number | null;
  mediumValidrate: number | null;
  easyValidrate: number | null;
};

const extractDifficultyRows = (raw: unknown): LlmByDifficulty[] => {
  const source = raw as any;
  if (!source) return [];
  if (Array.isArray(source.llms)) return source.llms as LlmByDifficulty[];
  if (Array.isArray(source.data?.llms)) return source.data.llms as LlmByDifficulty[];
  return [];
};

const combined_difficulty_columns: GridColDef<any>[] = [
  {
    field: 'name',
    headerName: 'Model',
    flex: 1.2,
    minWidth: 200,
    align: 'left',
    headerAlign: 'center',
    sortable: false,
    renderCell: (params) => (
      <Stack spacing={0.5} sx={{ py: 0.75 }}>
        <Typography variant="body2" sx={{ fontWeight: 600 }}>{get_llm_show_name(params.row)}</Typography>
        <Typography
          variant="caption"
          sx={{ color: 'text.secondary', textTransform: 'uppercase', letterSpacing: '0.08em' }}
        >
          {params.row.organization ?? params.row.provider}
        </Typography>
      </Stack>
    ),
  },
  {
    field: 'hardPassrate',
    headerName: 'Hard',
    flex: 0.9,
    minWidth: 170,
    align: 'left',
    headerAlign: 'center',
    renderCell: renderPassCell('#f85149'),
    sortable: true,
  },
  {
    field: 'mediumPassrate',
    headerName: 'Medium',
    flex: 0.9,
    minWidth: 170,
    align: 'left',
    headerAlign: 'center',
    renderCell: renderPassCell('#c9d1d9'),
    sortable: true,
  },
  {
    field: 'easyPassrate',
    headerName: 'Easy',
    flex: 0.9,
    minWidth: 170,
    align: 'left',
    headerAlign: 'center',
    renderCell: renderPassCell('#8b949e'),
    sortable: true,
  },
  {
    field: 'rating',
    headerName: 'Rating',
    flex: 0.6,
    minWidth: 110,
    align: 'right',
    headerAlign: 'center',
    renderCell: (params) => (
      <Stack spacing={0} alignItems="flex-end" sx={{ width: '100%' }}>
        <Typography variant="body1" sx={{ fontWeight: 700, fontSize: '1.05rem', color: '#0f172a' }}>
          {(() => {
            const ratingValue = computeRating(params.row ?? {});
            if (Number.isFinite(ratingValue) && ratingValue > 0) {
              return Math.round(ratingValue as number);
            }
            const rawRating = params.row?.rawRating ?? params.row?.rating;
            if (typeof rawRating === 'string' && rawRating.trim() !== '') {
              return rawRating;
            }
            return '—';
          })()}
        </Typography>
        <Typography variant="caption" sx={{ color: 'text.secondary', fontSize: 11 }}>
          Elo
        </Typography>
      </Stack>
    ),
  },
];

interface Contest {
	id: number,
	contestId: number,
	time: number,
}

type LeaderboardProps = {
  showTable?: boolean;
  showRatingTrend?: boolean;
};

export const Leaderboard: React.FC<LeaderboardProps> = ({
  showTable = true,
  showRatingTrend = false,
}) => {
  const list_result = useRequest(listLlmLeaderboardLlmGet);
  const hard_result = useRequest(listLlmByDifficultyLeaderboardLlmDifficultyGet, { manual: true });
  const medium_result = useRequest(listLlmByDifficultyLeaderboardLlmDifficultyGet, { manual: true });
  const easy_result = useRequest(listLlmByDifficultyLeaderboardLlmDifficultyGet, { manual: true });

  const [ open, setOpen ] = useState(false);
  const [ selected, setSelected ] = useState<{name: string; provider: string; bench_mode: UiBenchmarkMode} | null>(null);
  const [ benchMode, setBenchMode ] = useState('live' as UiBenchmarkMode);
  const [ disableSet, setDisableSet ] = useState(new Set<string>());
  const [allSelected, setAllSelected] = useState(false);
  const [initialLoadDone, setInitialLoadDone] = useState(false);
  const [modelSearchTerm, setModelSearchTerm] = useState("");
  const [modelSelectionExpanded, setModelSelectionExpanded] = useState(false);


  const hard_run = hard_result?.run;
  const medium_run = medium_result?.run;
  const easy_run = easy_result?.run;

  const allModels: Llm[] = useMemo(() => {
    const raw = list_result.data as any;
    if (!raw) return [];
    if (Array.isArray(raw)) return raw as Llm[];
    if (Array.isArray(raw.data)) return raw.data as Llm[];
    return [];
  }, [list_result.data]);

  const active_data: Llm[] = useMemo(
    () => allModels.filter((model: Llm) => model.status === 'active'),
    [allModels],
  );
  const loading = list_result?.loading;
  const error = list_result?.error;
  const combined_difficulty_loading = showTable
    ? hard_result.loading || medium_result.loading || easy_result.loading
    : false;
  
  // Combined data structure for all difficulties
  const combined_difficulty_data: CombinedLeaderboardRow[] = useMemo(() => {
    if (!showTable) {
      return [];
    }

    const hardData = extractDifficultyRows(hard_result.data).filter((item) => item.status === 'active');
    const mediumData = extractDifficultyRows(medium_result.data).filter((item) => item.status === 'active');
    const easyData = extractDifficultyRows(easy_result.data).filter((item) => item.status === 'active');

    const combinedMap = new Map<string, CombinedLeaderboardRow>();

    active_data.forEach((model: Llm) => {
      const key = `${model.provider}/${model.name}`;
      combinedMap.set(key, {
        ...model,
        rawRating: model.rating ?? null,
        rating: computeRating(model),
        hardPassrate: 0,
        mediumPassrate: 0,
        easyPassrate: 0,
        hardValidrate: null,
        mediumValidrate: null,
        easyValidrate: null,
      });
    });

    const mergePassrate = (
      collection: LlmByDifficulty[],
      passField: 'hardPassrate' | 'mediumPassrate' | 'easyPassrate',
      validField: 'hardValidrate' | 'mediumValidrate' | 'easyValidrate',
    ) => {
      collection.forEach((model: LlmByDifficulty) => {
        const key = `${model.provider}/${model.name}`;
        const existing = combinedMap.get(key);
        if (existing) {
          existing[passField] = model.passrate;
          existing[validField] = model.validrate;
        } else {
          combinedMap.set(key, {
            ...model,
            rawRating: model.rating ?? null,
            rating: computeRating(model),
            hardPassrate: passField === 'hardPassrate' ? model.passrate : 0,
            mediumPassrate: passField === 'mediumPassrate' ? model.passrate : 0,
            easyPassrate: passField === 'easyPassrate' ? model.passrate : 0,
            hardValidrate: validField === 'hardValidrate' ? model.validrate : null,
            mediumValidrate: validField === 'mediumValidrate' ? model.validrate : null,
            easyValidrate: validField === 'easyValidrate' ? model.validrate : null,
          } as CombinedLeaderboardRow);
        }
      });
    };

    mergePassrate(hardData, 'hardPassrate', 'hardValidrate');
    mergePassrate(mediumData, 'mediumPassrate', 'mediumValidrate');
    mergePassrate(easyData, 'easyPassrate', 'easyValidrate');

    const isStaticView = benchMode === 'static' || benchMode === 'static_2024q4' || benchMode === 'static_2025q1' || benchMode === 'static_2025q2' || benchMode === 'static_2025q3';

    const filteredRows = isStaticView
      ? Array.from(combinedMap.values()).filter((row) => {
          const validRates = [row.hardValidrate, row.mediumValidrate, row.easyValidrate];
          return validRates.every((rate) => rate === null || rate >= 0.8);
        })
      : Array.from(combinedMap.values());

    const sortedRows = filteredRows.sort((a, b) => {
      if (a.hardPassrate !== b.hardPassrate) {
        return b.hardPassrate - a.hardPassrate;
      }
      if (a.mediumPassrate !== b.mediumPassrate) {
        return b.mediumPassrate - a.mediumPassrate;
      }
    if (a.easyPassrate !== b.easyPassrate) {
      return b.easyPassrate - a.easyPassrate;
    }
    const ratingA = computeRating(a);
    const ratingB = computeRating(b);
    if (Number.isFinite(ratingA) && Number.isFinite(ratingB)) {
      return (ratingB as number) - (ratingA as number);
    }
    if (Number.isFinite(ratingB)) {
      return 1;
    }
    if (Number.isFinite(ratingA)) {
      return -1;
    }
    return 0;
  });

    return sortedRows;
  }, [hard_result.data, medium_result.data, easy_result.data, active_data, showTable, benchMode]);

  const valid_contest = useMemo(() => {
	  const id_list = active_data
		.filter((llm: Llm) => Array.isArray(llm.ratingEvent) && llm.ratingEvent.length > 0)
		.flatMap((llm: Llm) =>
			llm.ratingEvent!.map((event) => ({ contestId: event.contestId ?? 0, time: event.time })),
		);

	  const id_unique_list: Contest[] = [];
	  id_list.forEach((entry) => {
		  const exists = id_unique_list.find(
			  (current_contest) => entry.time === current_contest.time && entry.contestId === current_contest.contestId,
		  );
		  if (!exists) {
			  id_unique_list.push({
				  id: id_unique_list.length,
				  contestId: entry.contestId,
				  time: entry.time,
			  });
		  }
	  });
	  id_unique_list.sort((x, y) => x.time - y.time);
	  return id_unique_list;
  }, [active_data]);

  const raw_chart_data = useMemo(() => {
	  const id_list = valid_contest;
	  const result = active_data.map((model: Llm, idx: number) => {
		  if (!Array.isArray(model.ratingEvent) || model.ratingEvent.length === 0) {
			  return {
				  id: `${model.provider}/${model.name}`,
				  label: get_llm_show_name(model),
				  data: [],
			  };
		  }
		  const series: number[] = [];
		  id_list.forEach((event: Contest) => {
      const current = model.ratingEvent!.find((current_event) => current_event.contestId === event.contestId);
      const currentRating = parseRatingValue(current?.rating);
      if (currentRating !== null) {
        series.push(currentRating);
      } else {
        const fallback = series.length === 0 ? computeRating(model) : series[series.length - 1];
        series.push(fallback);
      }
		  });
		  return {
			  id: `${model.provider}/${model.name}`,
			  label: get_llm_show_name(model),
			  data: series,
			  color: geekTheme.palette.chartColors[idx % geekTheme.palette.chartColors.length],
		  };
	  });
	  return result;
  }, [active_data, valid_contest]);

  const chart_data = useMemo(() => {
	  return raw_chart_data.filter( x => !disableSet.has(x.label) );
  }, [raw_chart_data, disableSet]);

  const filteredModelsForSelection = useMemo(() => {
    if (!modelSearchTerm) {
      return raw_chart_data;
    }
    return raw_chart_data.filter(model => 
      model.label.toLowerCase().includes(modelSearchTerm.toLowerCase())
    );
  }, [raw_chart_data, modelSearchTerm]);



  useEffect(() => {
    if (!showTable) return;

	  const apiMode = getApiBenchmarkMode(benchMode);
	  const problemSet = getProblemSetName(benchMode);
	  
	  hard_run?.({
		  query: {
			  difficulty: 'hard', 
			  benchmark_mode: apiMode,
			  static_problemset: problemSet
		  }
	  });
	  medium_run?.({
		  query: {
			  difficulty: 'medium', 
			  benchmark_mode: apiMode,
			  static_problemset: problemSet
		  }
	  });
	  easy_run?.({
		  query: {
			  difficulty: 'easy', 
			  benchmark_mode: apiMode,
			  static_problemset: problemSet
		  }
	  });
  }, [benchMode, hard_run, medium_run, easy_run, showTable]);

  // Initialize top 5 models when data is loaded, ONLY ONCE
  useEffect(() => {
    // Check if data is loaded AND initialization hasn't happened yet
    if (active_data.length > 0 && !initialLoadDone) {
      // Sort models by rating (descending)
      const sortedModels = [...active_data].sort((a, b) => {
        const ratingA = computeRating(a);
        const ratingB = computeRating(b);
        return ratingB - ratingA;
      });

      // Get all model names
      const allModelLabels = active_data.map(model => get_llm_show_name(model));

      // Get top 5 model names
      const topFiveLabels = sortedModels.slice(0, 5).map(model => get_llm_show_name(model));

      // Create set of models to disable (all except top 5)
      const labelsToDisable = allModelLabels.filter(name => !topFiveLabels.includes(name));

      setDisableSet(new Set(labelsToDisable));
      setInitialLoadDone(true); // Mark initialization as done
    }
  }, [active_data, initialLoadDone]);

  const handleBenchChange = (_event: React.SyntheticEvent, newValue: string) => {
    setBenchMode(newValue as UiBenchmarkMode);
  };

  const toggleAllModels = () => {
    if (allSelected) {
      // Deselect all
      const allModels = raw_chart_data.map((x: { label: string }) => x.label);
      setDisableSet(new Set(allModels));
      setAllSelected(false);
    } else {
      // Select all
      setDisableSet(new Set());
      setAllSelected(true);
    }
  };

  const getSelectedCount = () => {
    return raw_chart_data.length - disableSet.size;
  };

  return (
    <Box sx={{
      p: { xs: 1, sm: 2, md: 3 },
      width: '100%',
      overflowX: 'hidden',
    }}>
      <Stack spacing={{ xs: 4, md: 6 }}>
        {showTable && (
          <Stack spacing={3}>
            <TabContext value={benchMode}>
              <Box
                sx={{
                  borderBottom: 1,
                  borderColor: 'divider',
                  mb: 2,
                  display: 'flex',
                  alignItems: 'center',
                  gap: 1,
                  flexWrap: 'wrap',
                }}
              >
                <TabList onChange={handleBenchChange} aria-label="Benchmark mode tab">
                  <Tab label="Live" value="live" />
                  <Tab label="24Q4" value="static_2024q4" />
                  <Tab label="25Q1" value="static_2025q1" />
                  <Tab label="25Q2" value="static_2025q2" />
                  <Tab label="25Q3" value="static_2025q3" />
                </TabList>
              </Box>
            </TabContext>

            <Paper
              elevation={0}
              sx={{
                minHeight: { xs: '30vh', sm: '40vh' },
                position: 'relative',
                overflowX: 'auto',
                '& .MuiDataGrid-root': {
                  minWidth: { xs: '640px', sm: 'auto' },
                },
              }}
            >
              {combined_difficulty_loading && (
                <Box
                  sx={{
                    position: 'absolute',
                    inset: 0,
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'center',
                    bgcolor: 'rgba(13, 17, 23, 0.6)',
                    zIndex: 1,
                  }}
                >
                  <CircularProgress size={32} />
                </Box>
              )}
              <DataGrid
                rows={combined_difficulty_data}
                style={combined_difficulty_loading ? { filter: 'blur(3px)' } : undefined}
                columns={combined_difficulty_columns}
                getRowId={(x) => `${x.provider}/${x.name}`}
                disableRowSelectionOnClick
                disableColumnMenu
                disableColumnSelector
                density="comfortable"
                getRowHeight={() => 68}
                hideFooter
                onRowClick={(params) => {
                  const row = params.row;
                  setSelected({ name: row.name, provider: row.provider, bench_mode: benchMode });
                  setOpen(true);
                }}
                sx={{
                  border: 'none',
                  '--DataGrid-rowBorderColor': 'rgba(15, 23, 42, 0.08)',
                  '& .MuiDataGrid-columnHeaders': {
                    backgroundColor: 'rgba(15, 23, 42, 0.04)',
                    textTransform: 'uppercase',
                    letterSpacing: '0.12em',
                    fontSize: 12.5,
                    color: 'text.secondary',
                    borderBottom: '1px solid rgba(15, 23, 42, 0.1)',
                  },
                  '& .MuiDataGrid-columnHeader': {
                    py: { xs: 1.1, sm: 1.4 },
                    display: 'flex',
                    justifyContent: 'center',
                  },
                  '& .MuiDataGrid-columnHeaderTitle': {
                    fontWeight: 600,
                    width: '100%',
                    textAlign: 'center',
                  },
                  '& .MuiDataGrid-cell': {
                    fontSize: { xs: '0.92rem', sm: '0.98rem' },
                    alignItems: 'center',
                    borderBottom: '1px solid rgba(148, 163, 184, 0.18)',
                  },
                  '& .MuiDataGrid-row': {
                    transition: 'background-color 120ms ease',
                    borderBottom: '1px solid rgba(148, 163, 184, 0.18)',
                  },
                }}
              />
            </Paper>

            <Modal open={open} onClose={() => setOpen(false)}>
              <Box
                sx={{
                  position: 'absolute',
                  top: { xs: '5%', sm: '50%' },
                  left: '50%',
                  transform: { xs: 'translate(-50%, 0)', sm: 'translate(-50%, -50%)' },
                  bgcolor: 'background.paper',
                  border: '1px solid',
                  borderColor: 'divider',
                  boxShadow: 24,
                  p: { xs: 2, sm: 4 },
                  width: { xs: '95vw', sm: '90vw' },
                  maxWidth: { xs: 'none', sm: '90vw' },
                  height: { xs: '90vh', sm: 'auto' },
                  maxHeight: { xs: '90vh', sm: '90vh' },
                  overflow: 'auto',
                  borderRadius: { xs: 2, sm: 2 },
                }}
              >
                {selected && (
                  <CombinedSubmissionsViewer
                    name={selected.name}
                    provider={selected.provider}
                    benchMode={getApiBenchmarkMode(selected.bench_mode)}
                    originalBenchMode={selected.bench_mode}
                  />
                )}
              </Box>
            </Modal>

            <Stack spacing={1}>
              <Typography
                id="difficulty"
                variant="body2"
                sx={{ color: 'text.secondary' }}
              >
                Click a row to inspect per-problem submissions. Rankings prioritise hard success rate, then medium,
                easy, and overall rating. Ratings reflect Codeforces-style Elo updates from the benchmark API. We
                publish updated splits each quarter (e.g. 24Q4, 25Q1, 25Q2). Live tracks use the latest rotating problem
                window.
              </Typography>
            </Stack>
          </Stack>
        )}

        {showTable && showRatingTrend && (
          <Box sx={{ borderTop: '1px solid', borderColor: 'divider', pt: { xs: 2, md: 3 } }} />
        )}

        {showRatingTrend && (
          <Stack spacing={3}>
            <Stack spacing={1}>
              <Typography variant="body2" sx={{ color: 'text.secondary' }}>
                Each model competes as a virtual contestant within actual Codeforces rounds. When a model solves a
                problem, its penalty time is set to the human median; Elo is then recalculated using the original
                standings and contestant ratings, matching Codeforces methodology. We publish updated splits each quarter
                (e.g. 24Q4, 25Q1, 25Q2). Live tracks use the latest rotating problem window.
              </Typography>
            </Stack>

            {loading && (
              <Box sx={{ display: 'flex', alignItems: 'center', justifyContent: 'center', minHeight: 160 }}>
                <CircularProgress size={28} />
              </Box>
            )}
            {error && <Alert severity="error">{error.message}</Alert>}

            {active_data && (
              <Paper sx={{ p: { xs: 1, sm: 2 }, overflowX: 'auto' }}>
                <LineChart
                  xAxis={[
                    {
                      data: valid_contest.map((x: Contest) => new Date(x.time * 1000)),
                      scaleType: 'time',
                      valueFormatter: (value: Date) =>
                        value.toLocaleDateString('en-US', {
                          year: 'numeric',
                          month: 'short',
                          day: 'numeric',
                        }),
                    },
                  ]}
                  yAxis={[{ id: 'Rating', scaleType: 'linear' }]}
                  series={chart_data.map((x: { id: string; label: string; data: number[]; color?: string }) => ({
                    curve: 'linear',
                    showMark: false,
                    ...x,
                  }))}
                  height={300}
                  slotProps={{ legend: { hidden: true } }}
                  slots={{ axisContent: CustomAxisTooltip }}
                  margin={{ left: 50, right: 20, top: 20, bottom: 60 }}
                />

                <Accordion
                  expanded={modelSelectionExpanded}
                  onChange={() => setModelSelectionExpanded(!modelSelectionExpanded)}
                  sx={{
                    mt: { xs: 2, sm: 3 },
                    border: '1px solid rgba(104, 211, 145, 0.35)',
                    borderRadius: '12px !important',
                    bgcolor: 'rgba(72, 187, 120, 0.05)',
                    '&:before': { display: 'none' },
                  }}
                >
                  <AccordionSummary
                    expandIcon={<FaChevronDown style={{ color: '#48bb78' }} />}
                    sx={{
                      px: { xs: 2, sm: 3 },
                      '& .MuiAccordionSummary-content': {
                        alignItems: 'center',
                        justifyContent: 'space-between',
                        gap: 2,
                      },
                      backgroundColor: 'rgba(72, 187, 120, 0.08)',
                      borderRadius: '12px',
                    }}
                  >
                    <Box sx={{ display: 'flex', alignItems: 'center', gap: 2 }}>
                      <Typography variant="subtitle1" sx={{ fontWeight: 600, color: '#68d391' }}>
                        Model Selection
                      </Typography>
                      <Chip
                        label={`${getSelectedCount()}/${raw_chart_data.length} selected`}
                        size="small"
                        color="navLink"
                        variant="outlined"
                        sx={{
                          fontWeight: 500,
                          fontSize: '0.75rem',
                          backgroundColor: 'rgba(72, 187, 120, 0.18)',
                          borderColor: 'rgba(72, 187, 120, 0.35)',
                          color: '#68d391',
                        }}
                      />
                    </Box>
                    <Box sx={{ display: 'flex', alignItems: 'center', gap: 1 }}>
                      {modelSelectionExpanded ? (
                        <FaEye style={{ color: '#68d391', fontSize: '1rem' }} />
                      ) : (
                        <FaEyeSlash style={{ color: '#68d391', fontSize: '1rem', opacity: 0.6 }} />
                      )}
                    </Box>
                  </AccordionSummary>
                  <AccordionDetails
                    sx={{
                      pt: 3,
                      px: { xs: 2, sm: 3 },
                      pb: { xs: 3, sm: 4 },
                      backgroundColor: '#ffffff',
                      borderTop: '1px solid rgba(104, 211, 145, 0.25)',
                    }}
                  >
                    <Box
                      sx={{
                        display: 'flex',
                        gap: { xs: 1.5, sm: 2 },
                        mb: { xs: 2, sm: 3 },
                        alignItems: 'center',
                        flexWrap: 'wrap',
                        flexDirection: { xs: 'column', sm: 'row' },
                      }}
                    >
                      <TextField
                        variant="outlined"
                        size="small"
                        placeholder="Search models..."
                        value={modelSearchTerm}
                        onChange={(e) => setModelSearchTerm(e.target.value)}
                        sx={{ flex: 1, minWidth: { xs: '100%', sm: '200px' }, width: { xs: '100%', sm: 'auto' } }}
                        InputProps={{
                          startAdornment: (
                            <InputAdornment position="start">
                              <FaSearch style={{ color: '#8b949e', fontSize: '1.1rem' }} />
                            </InputAdornment>
                          ),
                        }}
                      />
                      <Button
                        variant={allSelected ? 'contained' : 'outlined'}
                        color="navPurple"
                        onClick={toggleAllModels}
                        sx={{
                          minWidth: { xs: '100%', sm: '120px' },
                          width: { xs: '100%', sm: 'auto' },
                          fontWeight: 500,
                        }}
                        startIcon={allSelected ? <FaEyeSlash /> : <FaEye />}
                      >
                        {allSelected ? 'Hide All' : 'Show All'}
                      </Button>
                    </Box>

                    <Box
                      sx={{
                        display: 'grid',
                        gridTemplateColumns: {
                          xs: '1fr',
                          sm: 'repeat(auto-fill, minmax(250px, 1fr))',
                          md: 'repeat(auto-fill, minmax(280px, 1fr))',
                        },
                        gap: { xs: 1.5, sm: 2 },
                        maxHeight: { xs: '300px', sm: '400px' },
                        overflowY: 'auto',
                        pr: 1,
                        '&::-webkit-scrollbar': { width: '6px' },
                        '&::-webkit-scrollbar-track': {
                          backgroundColor: 'rgba(240, 246, 252, 0.04)',
                          borderRadius: '3px',
                        },
                        '&::-webkit-scrollbar-thumb': {
                          backgroundColor: 'rgba(240, 246, 252, 0.25)',
                          borderRadius: '3px',
                          '&:hover': {
                            backgroundColor: 'rgba(240, 246, 252, 0.4)',
                          },
                        },
                      }}
                    >
                      {filteredModelsForSelection.map((model) => {
                        const isSelected = !disableSet.has(model.label);
                        return (
                          <Paper
                            key={model.id}
                            variant="outlined"
                            sx={{
                              p: { xs: 1.5, sm: 2 },
                              cursor: 'pointer',
                              transition: 'all 0.2s ease-in-out',
                              backgroundColor: isSelected
                                ? 'rgba(240, 246, 252, 0.08)'
                                : 'rgba(240, 246, 252, 0.02)',
                              borderColor: isSelected ? model.color || '#f0f6fc' : 'rgba(240, 246, 252, 0.1)',
                              borderWidth: isSelected ? '2px' : '1px',
                              '&:hover': {
                                backgroundColor: isSelected
                                  ? 'rgba(240, 246, 252, 0.14)'
                                  : 'rgba(240, 246, 252, 0.06)',
                              borderColor: model.color || '#f0f6fc',
                              transform: { xs: 'none', sm: 'translateY(-1px)' },
                              boxShadow: '0 4px 12px rgba(240, 246, 252, 0.15)',
                              },
                            }}
                            onClick={() => {
                              const updated = new Set(disableSet);
                              if (isSelected) {
                                updated.add(model.label);
                              } else {
                                updated.delete(model.label);
                              }
                              setDisableSet(updated);
                              setAllSelected(updated.size === 0);
                            }}
                          >
                            <Box sx={{ display: 'flex', alignItems: 'center', gap: { xs: 1, sm: 1.5 } }}>
                              <Box
                                sx={{
                                  width: { xs: 10, sm: 12 },
                                  height: { xs: 10, sm: 12 },
                                  borderRadius: '50%',
                                  backgroundColor: model.color || 'primary.main',
                                  flexShrink: 0,
                                  opacity: isSelected ? 1 : 0.3,
                                  transition: 'opacity 0.2s ease-in-out',
                                }}
                              />

                              <Typography
                                variant="body2"
                                sx={{
                                  fontWeight: isSelected ? 'medium' : 'normal',
                                  color: isSelected ? 'text.primary' : 'text.secondary',
                                  flex: 1,
                                  transition: 'all 0.2s ease-in-out',
                                  fontSize: { xs: '0.8rem', sm: '0.875rem' },
                                }}
                              >
                                {model.label}
                              </Typography>

                              {isSelected ? (
                                <FaEye style={{ color: '#f0f6fc', fontSize: '0.9rem', opacity: 0.85 }} />
                              ) : (
                                <FaEyeSlash style={{ color: '#6e7681', fontSize: '0.9rem', opacity: 0.6 }} />
                              )}
                            </Box>
                          </Paper>
                        );
                      })}
                    </Box>

                    <Box
                      sx={{
                        mt: 3,
                        pt: 2,
                        borderTop: '1px solid',
                        borderColor: 'divider',
                        display: 'flex',
                        justifyContent: 'space-between',
                        alignItems: 'center',
                        flexWrap: 'wrap',
                        gap: 2,
                      }}
                    >
                      <Typography variant="body2" sx={{ color: 'text.secondary' }}>
                        {modelSearchTerm && (
                          <>Showing {filteredModelsForSelection.length} of {raw_chart_data.length} models • </>
                        )}
                        {getSelectedCount()} models visible in chart
                      </Typography>
                      {modelSearchTerm && (
                        <Button size="small" onClick={() => setModelSearchTerm('')} sx={{ color: 'text.secondary' }}>
                          Clear search
                        </Button>
                      )}
                    </Box>
                  </AccordionDetails>
                </Accordion>
              </Paper>
            )}
          </Stack>
        )}
      </Stack>
    </Box>
  );

};

export const RatingTrendPage: React.FC = () => (
  <Leaderboard showTable={false} showRatingTrend />
);
