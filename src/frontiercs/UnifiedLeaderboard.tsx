import React, { useState, useMemo, useCallback } from 'react';
import {
  Alert,
  Box,
  CircularProgress,
  Paper,
  Stack,
  Typography,
  ToggleButtonGroup,
  ToggleButton,
} from '@mui/material';
import { useRequest } from 'ahooks';
import { dataService } from '../data';
import { BarChart } from '@mui/x-charts/BarChart';
import type { BarLabelProps } from '@mui/x-charts/BarChart';
import type { ChartsItemContentProps } from '@mui/x-charts/ChartsTooltip/ChartsItemTooltipContent';
import { ChartsText } from '@mui/x-charts/ChartsText';
import type { ChartsTextProps } from '@mui/x-charts/ChartsText';

const OverviewBarLabel: React.FC<BarLabelProps> = ({ style, children, ...rest }) => {
  const resolvedStyle = style as any;
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
  const formattedScoreText = shouldRenderScore ? `${formattedScoreValue.toFixed(1)}` : undefined;

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

type ProblemType = 'research' | 'competitive';

export const UnifiedLeaderboard: React.FC = () => {
  const [problemType, setProblemType] = useState<ProblemType>('research');

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
  const chartRows = useMemo(() => {
    const leaderboard = problemType === 'research' ? researchLeaderboard : cpLeaderboard;
    if (!leaderboard) return [];

    return leaderboard
      .sort((a, b) => b.avgScore - a.avgScore)
      .map((model) => {
        const isCP = problemType === 'competitive';
        const cpModel = isCP ? (model as any) : null;

        return {
          id: `${model.provider}/${model.name}`,
          label: model.name,
          organization: model.provider ?? '—',
          value: model.avgScore,
          passRate: cpModel?.passRate,
          totalAttempts: cpModel?.totalAttempts,
          totalProblems: isCP ? cpModel?.totalProblems : (model as any).totalProblems,
          attempted: isCP ? cpModel?.totalAttempts : (model as any).attempted,
        };
      });
  }, [problemType, researchLeaderboard, cpLeaderboard]);

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

      const formattedValue = Number.isFinite(row.value) ? `${row.value.toFixed(1)}` : '0.0';
      const isCP = problemType === 'competitive';

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
            minWidth: 240,
          }}
        >
          <Stack spacing={1}>
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
                  Avg Score
                </Typography>
                <Typography variant="body2" sx={{ fontWeight: 600, color: '#0f172a' }}>
                  {formattedValue}
                </Typography>
              </Stack>
            </Stack>
            {isCP && row.passRate !== undefined && row.totalAttempts !== undefined && (
              <Stack direction="row" justifyContent="space-between" spacing={2}>
                <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                  Pass Rate: <strong>{row.passRate.toFixed(2)}%</strong>
                </Typography>
                <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                  Attempts: <strong>{row.totalAttempts}</strong>
                </Typography>
              </Stack>
            )}
            {!isCP && (
              <Stack direction="row" justifyContent="space-between" spacing={2}>
                <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                  Problems: <strong>{row.attempted}/{row.totalProblems}</strong>
                </Typography>
              </Stack>
            )}
          </Stack>
        </Paper>
      );
    },
    [chartRows, problemType],
  );

  const barTooltipConfig = useMemo(
    () => ({
      trigger: 'item' as const,
    }),
    [],
  );

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

  const totalSubmissions = problemType === 'research'
    ? (researchResults?.results.length || 0)
    : (cpResults?.results.length || 0);

  const totalProblems = problemType === 'research'
    ? (researchResults?.problems.length || 0)
    : (cpResults?.problems.length || 0);

  return (
    <Stack spacing={3}>
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
                Frontier-CS Leaderboard
              </Typography>
              <Typography variant="caption" sx={{ color: 'text.secondary' }}>
                {totalSubmissions} total submissions across {totalProblems} problems
              </Typography>
            </Stack>
            <ToggleButtonGroup
              value={problemType}
              exclusive
              onChange={(_, value) => {
                if (value !== null) setProblemType(value);
              }}
              size="small"
              sx={{
                width: { xs: '100%', md: 'auto' },
                '& .MuiToggleButton-root': {
                  px: 2,
                  py: 0.5,
                  fontSize: 13,
                  textTransform: 'none',
                },
              }}
            >
              <ToggleButton value="research">Research Problems</ToggleButton>
              <ToggleButton value="competitive">Competitive Problems</ToggleButton>
            </ToggleButtonGroup>
          </Stack>

          {chartRows.length === 0 && (
            <Typography variant="body2" sx={{ color: 'text.secondary' }}>
              No models available for this problem type.
            </Typography>
          )}

          {chartRows.length > 0 && (
            <BarChart
              height={Math.max(320, chartRows.length * 44)}
              layout="horizontal"
              skipAnimation={false}
              barLabel={({ dataIndex }) => {
                const row = chartRows[dataIndex];
                if (!row) return '0.0';
                return `${row.value.toFixed(1)}`;
              }}
              series={[
                {
                  data: chartRows.map((row) => row.value),
                  label: 'Average Score',
                  color: problemType === 'research' ? '#8b5cf6' : '#16a34a',
                  valueFormatter: (_value, context) => {
                    const row = chartRows[context.dataIndex];
                    if (!row) return '0.0';
                    return `${row.value.toFixed(1)}`;
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
              margin={{ top: 16, bottom: 56, left: 180, right: 32 }}
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
