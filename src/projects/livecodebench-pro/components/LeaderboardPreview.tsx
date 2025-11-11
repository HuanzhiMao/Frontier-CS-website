import React, { useCallback, useEffect, useMemo, useRef, useState } from 'react';
import {
  Alert,
  Box,
  CircularProgress,
  Paper,
  Stack,
  ToggleButton,
  ToggleButtonGroup,
  Typography,
} from '@mui/material';
import { BarChart } from '@mui/x-charts/BarChart';
import type { BarLabelProps } from '@mui/x-charts/BarChart';
import type { ChartsItemContentProps } from '@mui/x-charts/ChartsTooltip/ChartsItemTooltipContent';
import { ChartsText } from '@mui/x-charts/ChartsText';
import type { ChartsTextProps } from '@mui/x-charts/ChartsText';
import { useRequest } from 'ahooks';

import {
  BenchmarkMode,
  listLlmByDifficultyLeaderboardLlmDifficultyGet,
} from '../../../api';

type Difficulty = 'hard' | 'medium' | 'easy';
type UiBenchmarkMode = BenchmarkMode | 'static_2024q4' | 'static_2025q1' | 'static_2025q2' | 'static_2025q3';

const difficultyLabels: Record<Difficulty, string> = {
  hard: 'Hard',
  medium: 'Medium',
  easy: 'Easy',
};

const benchModeOptions: Array<{ label: string; value: UiBenchmarkMode }> = [
  { label: 'Live', value: 'live' },
  { label: '24Q4', value: 'static_2024q4' },
  { label: '25Q1', value: 'static_2025q1' },
  { label: '25Q2', value: 'static_2025q2' },
  { label: '25Q3', value: 'static_2025q3' },
];

type BarLabelStyle = React.CSSProperties & {
  x?: number;
  y?: number;
  width?: number;
};

const OverviewBarLabel: React.FC<BarLabelProps> = ({ style, children, ...rest }) => {
  const resolvedStyle = style as BarLabelStyle | undefined;
  const x = typeof resolvedStyle?.x === 'number' ? resolvedStyle.x : 0;
  const width = typeof resolvedStyle?.width === 'number' ? resolvedStyle.width : 0;
  const y = typeof resolvedStyle?.y === 'number' ? resolvedStyle.y : 0;

  return (
    <text
      {...rest}
      x={x - width / 2 + 8}
      y={y}
      fill="#f8fafc"
      fontSize={13}
      fontWeight={600}
      textAnchor="start"
      dominantBaseline="central"
    >
      {typeof children === 'string' ? children : undefined}
    </text>
  );
};

type YAxisTickLabelProps = ChartsTextProps & {
  scoreMap?: Record<string, number>;
};

const OverviewYAxisTickLabel: React.FC<YAxisTickLabelProps> = ({ scoreMap, style, text, ...rest }) => {
  const label = typeof text === 'string' ? text : `${text ?? ''}`;
  const score = label ? scoreMap?.[label] : undefined;
  const formattedScoreValue =
    typeof score === 'number' && Number.isFinite(score) ? Number(score.toFixed(1)) : undefined;
  const shouldRenderScore = formattedScoreValue !== undefined && formattedScoreValue !== 0;
  const formattedScoreText = shouldRenderScore ? `${formattedScoreValue.toFixed(1)}%` : undefined;

  const scoreXOffset = 18;

  const chartTextProps = rest as ChartsTextProps & { ownerState?: unknown };
  const { ownerState: _ownerState, ...textElementProps } = chartTextProps;
  void _ownerState;

  const baseXValue = (chartTextProps as { x?: number | string }).x;
  const baseYValue = (chartTextProps as { y?: number | string }).y;

  const baseX = typeof baseXValue === 'number' ? baseXValue : Number(baseXValue ?? 0);
  const baseY = typeof baseYValue === 'number' ? baseYValue : Number(baseYValue ?? 0);

  return (
    <g>
      <ChartsText {...chartTextProps} style={style} text={label} />
      {shouldRenderScore && formattedScoreText && (
        <text
          {...textElementProps}
          x={baseX + scoreXOffset}
          y={baseY}
          style={{
            ...((style ?? {}) as React.CSSProperties),
            fill: '#f8fafc',
            textAnchor: 'start',
            pointerEvents: 'none',
          }}
          textAnchor="start"
        >
          {formattedScoreText}
        </text>
      )}
    </g>
  );
};

const resolveApiBenchmarkMode = (mode: UiBenchmarkMode): BenchmarkMode =>
  mode === 'static_2024q4' || mode === 'static_2025q1' || mode === 'static_2025q2' || mode === 'static_2025q3' ? 'static' : mode;

const resolveProblemSet = (mode: UiBenchmarkMode): string | undefined => {
  switch (mode) {
    case 'static_2024q4':
      return 'quater_2024_10_12';
    case 'static_2025q1':
      return 'quater_2025_1_3';
    case 'static_2025q2':
      return 'quater_2025_4_6';
    case 'static_2025q3':
      return 'quater_2025_7_9';
    default:
      return undefined;
  }
};

export interface LivecodeLeaderboardPreviewProps {
  autoScrollOnUpdate?: boolean;
  title?: string;
}

export const LivecodeLeaderboardPreview: React.FC<LivecodeLeaderboardPreviewProps> = ({
  autoScrollOnUpdate = false,
  title = 'Leaderboard',
}) => {
  const [difficulty, setDifficulty] = useState<Difficulty>('medium');
  const [benchMode, setBenchMode] = useState<UiBenchmarkMode>('live');
  const hasInitialised = useRef(false);

  const scrollToBottom = useCallback(() => {
    if (!autoScrollOnUpdate) return;
    window.requestAnimationFrame(() => {
      const { documentElement, body } = document;
      const scrollTarget = Math.max(
        documentElement?.scrollHeight ?? 0,
        body?.scrollHeight ?? 0,
      );
      window.scrollTo({ top: scrollTarget, behavior: 'smooth' });
    });
  }, [autoScrollOnUpdate]);

  const { data, loading, error, run } = useRequest(listLlmByDifficultyLeaderboardLlmDifficultyGet, {
    manual: true,
  });

  useEffect(() => {
    const apiMode = resolveApiBenchmarkMode(benchMode);
    const problemSet = resolveProblemSet(benchMode);
    run({
      query: {
        difficulty,
        benchmark_mode: apiMode,
        static_problemset: problemSet,
      },
    });
  }, [benchMode, difficulty, run]);

  const chartRows = useMemo(() => {
    const rows = data?.data?.llms?.filter((item) => item.status === 'active') ?? [];
    return rows
      .slice()
      .sort((a, b) => (b.passrate ?? 0) - (a.passrate ?? 0))
      .slice(0, 10)
      .map((model) => ({
        id: `${model.provider}/${model.name}`,
        label: model.display_name ?? model.name,
        organization: model.organization ?? model.provider ?? '—',
        value: (model.passrate ?? 0) * 100,
      }));
  }, [data]);

  const scoreLookup = useMemo(() => {
    const lookup: Record<string, number> = {};
    chartRows.forEach((row) => {
      lookup[row.label] = row.value;
    });
    return lookup;
  }, [chartRows]);

  const renderBarTooltip = useCallback(
    ({ itemData }: ChartsItemContentProps<'bar'>) => {
      const dataIndex = itemData.dataIndex ?? -1;
      if (dataIndex < 0) return null;
      const row = chartRows[dataIndex];
      if (!row) return null;

      const formattedValue = Number.isFinite(row.value) ? `${row.value.toFixed(1)}%` : '0.0%';

      return (
        <Paper
          elevation={0}
          sx={{
            px: 2,
            py: 1.5,
            bgcolor: '#ffffff',
            borderRadius: 2,
            border: '1px solid rgba(15, 23, 42, 0.12)',
            boxShadow: '0 12px 28px rgba(15, 23, 42, 0.18)',
            minWidth: 220,
          }}
        >
          <Stack direction="row" justifyContent="space-between" spacing={3} alignItems="flex-start">
            <Stack spacing={0.25} alignItems="flex-start">
              <Typography variant="body2" sx={{ fontWeight: 600, color: '#0f172a' }}>
                {row.label}
              </Typography>
              <Typography
                variant="caption"
                sx={{ color: 'text.secondary', textTransform: 'uppercase', letterSpacing: '0.08em' }}
              >
                {row.organization}
              </Typography>
            </Stack>
            <Stack spacing={0.25} alignItems="flex-end">
              <Typography variant="caption" sx={{ color: 'text.secondary', fontWeight: 600 }}>
                Pass@1
              </Typography>
              <Typography variant="body2" sx={{ fontWeight: 600, color: '#0f172a' }}>
                {formattedValue}
              </Typography>
            </Stack>
          </Stack>
        </Paper>
      );
    },
    [chartRows],
  );

  const barTooltipConfig = useMemo(
    () => ({
      trigger: 'item' as const,
    }),
    [],
  );

  useEffect(() => {
    if (!autoScrollOnUpdate) return;
    if (!hasInitialised.current) {
      hasInitialised.current = true;
      return;
    }
    scrollToBottom();
  }, [autoScrollOnUpdate, chartRows, scrollToBottom]);

  return (
    <Stack spacing={{ xs: 3, md: 4 }}>
      <Paper sx={{ p: { xs: 2, md: 3 } }}>
        <Stack spacing={2.5}>
          <Stack
            direction={{ xs: 'column', md: 'row' }}
            spacing={2}
            alignItems={{ xs: 'flex-start', md: 'center' }}
            justifyContent="space-between"
          >
            <Stack spacing={0.75} alignItems="flex-start">
              <Typography variant="h6" sx={{ fontWeight: 600 }}>
                {title}
              </Typography>
            </Stack>
            <Stack
              direction="column"
              spacing={0.75}
              alignItems="flex-end"
              sx={{ width: { xs: '100%', md: 'auto' } }}
            >
              <ToggleButtonGroup
                value={difficulty}
                exclusive
                onChange={(_, value: Difficulty | null) => {
                  if (value) {
                    setDifficulty(value);
                    scrollToBottom();
                  }
                }}
                size="small"
                aria-label="difficulty selector"
                sx={{
                  width: { xs: '100%', md: 'auto' },
                  '& .MuiToggleButton-root': {
                    px: 1,
                    py: 0.3,
                    fontSize: 12,
                  },
                }}
              >
                {(Object.keys(difficultyLabels) as Difficulty[]).map((key) => (
                  <ToggleButton key={key} value={key} aria-label={difficultyLabels[key]} sx={{ textTransform: 'none' }}>
                    {difficultyLabels[key]}
                  </ToggleButton>
                ))}
              </ToggleButtonGroup>
              <ToggleButtonGroup
                value={benchMode}
                exclusive
                onChange={(_, value: UiBenchmarkMode | null) => {
                  if (value) {
                    setBenchMode(value);
                    scrollToBottom();
                  }
                }}
                size="small"
                aria-label="benchmark mode selector"
                sx={{
                  width: { xs: '100%', md: 'auto' },
                  '& .MuiToggleButton-root': {
                    px: 1,
                    py: 0.3,
                    fontSize: 12,
                  },
                }}
              >
                {benchModeOptions.map((option) => (
                  <ToggleButton key={option.value} value={option.value} aria-label={option.label} sx={{ textTransform: 'none' }}>
                    {option.label}
                  </ToggleButton>
                ))}
              </ToggleButtonGroup>
            </Stack>
          </Stack>

          {loading && (
            <Box sx={{ display: 'flex', alignItems: 'center', justifyContent: 'center', minHeight: 240 }}>
              <CircularProgress size={28} />
            </Box>
          )}
          {error && <Alert severity="error">{error.message}</Alert>}
          {!loading && !error && chartRows.length === 0 && (
            <Typography variant="body2" sx={{ color: 'text.secondary' }}>
              No active models available for this slice yet.
            </Typography>
          )}
          {!loading && !error && chartRows.length > 0 && (
            <BarChart
              height={Math.max(320, chartRows.length * 44)}
              layout="horizontal"
              skipAnimation={false}
              barLabel={({ dataIndex }) => {
                const row = chartRows[dataIndex];
                if (!row) return '0.0%';
                return `${row.value.toFixed(1)}%`;
              }}
              series={[
                {
                  data: chartRows.map((row) => row.value),
                  label: 'Pass@1 (%)',
                  color: '#4338ca',
                  valueFormatter: (_value, context) => {
                    const row = chartRows[context.dataIndex];
                    if (!row) return '0.0%';
                    return `${row.value.toFixed(1)}%`;
                  },
                },
              ]}
              yAxis={[
                {
                  scaleType: 'band',
                  data: chartRows.map((row) => row.label),
                  tickLabelStyle: {
                    fill: '#1f2937',
                    fontWeight: 500,
                    fontSize: 12,
                    textTransform: 'none',
                  },
                },
              ]}
              xAxis={[{ min: 0, max: 100 }]}
              margin={{ top: 16, bottom: 56, left: 220, right: 32 }}
              tooltip={barTooltipConfig}
              slots={{
                barLabel: OverviewBarLabel,
                axisTickLabel: OverviewYAxisTickLabel,
                itemContent: renderBarTooltip,
              }}
              slotProps={{
                legend: { hidden: true },
                itemContent: { sx: { p: 0 } },
                popper: {
                  sx: {
                    pointerEvents: 'none',
                  },
                },
                axisTickLabel: {
                  scoreMap: scoreLookup,
                } as unknown as Partial<ChartsTextProps>,
              }}
            />
          )}
        </Stack>
      </Paper>
    </Stack>
  );
};
