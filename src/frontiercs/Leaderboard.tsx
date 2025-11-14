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
import { dataService, type CategoryName } from '../data';
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

export const FrontierCSLeaderboard: React.FC = () => {
  const [selectedCategory, setSelectedCategory] = useState<CategoryName | 'all'>('all');

  // Load leaderboard data
  const { data: leaderboard, loading, error } = useRequest(async () => {
    return await dataService.loadLeaderboard();
  });

  // Load category stats
  const { data: categoryStats } = useRequest(async () => {
    return await dataService.getCategoryStats();
  });

  // Filter models by selected category
  const chartRows = useMemo(() => {
    if (!leaderboard) return [];

    let models = leaderboard;

    if (selectedCategory !== 'all') {
      models = leaderboard.map(model => {
        const categoryResults = model.results.filter(r => {
          const problem = r.problem;
          const problemCat = categoryStats?.find(c =>
            c.problems.some(p => p.name === problem)
          )?.category;
          return problemCat === selectedCategory;
        });

        const validScores = categoryResults.filter(r => r.score !== null).map(r => r.score as number);
        const avgScore = validScores.length > 0
          ? validScores.reduce((a, b) => a + b, 0) / validScores.length
          : 0;

        return {
          ...model,
          attempted: categoryResults.length,
          avgScore: Number(avgScore.toFixed(2)),
          results: categoryResults
        };
      });
    }

    return models
      .sort((a, b) => b.avgScore - a.avgScore)
      .map((model) => ({
        id: `${model.provider}/${model.name}`,
        label: model.name,
        organization: model.provider ?? '—',
        value: model.avgScore,
        attempted: model.attempted,
      }));
  }, [leaderboard, selectedCategory, categoryStats]);

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
                Avg Score
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
            </Stack>
            <ToggleButtonGroup
              value={selectedCategory}
              exclusive
              onChange={(_, value) => {
                if (value !== null) setSelectedCategory(value);
              }}
              size="small"
              sx={{
                width: { xs: '100%', md: 'auto' },
                '& .MuiToggleButton-root': {
                  px: 1,
                  py: 0.3,
                  fontSize: 12,
                  textTransform: 'none',
                },
              }}
            >
              <ToggleButton value="all">All</ToggleButton>
              {categoryStats?.map(({ category }) => (
                <ToggleButton key={category} value={category}>
                  {category.split(' ').slice(0, 2).join(' ')}
                </ToggleButton>
              ))}
            </ToggleButtonGroup>
          </Stack>

          {chartRows.length === 0 && (
            <Typography variant="body2" sx={{ color: 'text.secondary' }}>
              No models available for this category.
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
                  color: '#4338ca',
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
