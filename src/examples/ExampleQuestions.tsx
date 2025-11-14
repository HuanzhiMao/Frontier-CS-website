import React, { useState, useMemo } from 'react';
import {
  Alert,
  Box,
  CircularProgress,
  Stack,
  Typography,
  Chip,
  Link as MuiLink,
  Accordion,
  AccordionSummary,
  AccordionDetails,
  ToggleButtonGroup,
  ToggleButton,
  Paper,
} from '@mui/material';
import { FaChevronDown, FaExternalLinkAlt, FaCheckCircle, FaTimesCircle } from 'react-icons/fa';
import { useRequest } from 'ahooks';
import {
  getLlmSubmissionsLeaderboardLlmSubmissionsGet,
  getSubmissionLeaderboardSubmissionSubmissionIdGet,
  listLlmByDifficultyLeaderboardLlmDifficultyGet,
  BenchmarkMode,
  ProblemDifficulty,
  LlmContestProblemSubmission,
} from '../api';

type UiBenchmarkMode = BenchmarkMode | 'static_2024q4' | 'static_2025q1' | 'static_2025q2' | 'static_2025q3';

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

const benchModeOptions: Array<{ label: string; value: UiBenchmarkMode }> = [
  { label: 'Live', value: 'live' },
  { label: '24Q4', value: 'static_2024q4' },
  { label: '25Q1', value: 'static_2025q1' },
  { label: '25Q2', value: 'static_2025q2' },
  { label: '25Q3', value: 'static_2025q3' },
];

interface ExampleSubmissionProps {
  problem: LlmContestProblemSubmission;
  modelName: string;
  modelProvider: string;
}

const ExampleSubmission: React.FC<ExampleSubmissionProps> = ({ problem, modelName, modelProvider }) => {
  const { data, loading, error } = useRequest(
    async () => {
      if (!problem.submission_id) return null;
      return await getSubmissionLeaderboardSubmissionSubmissionIdGet({
        path: { submission_id: problem.submission_id },
      });
    },
    {
      ready: !!problem.submission_id,
    }
  );

  const isAccepted = problem.verdict === 'accepted';
  const verdictColor = isAccepted ? 'success' : 'error';
  const VerdictIcon = isAccepted ? FaCheckCircle : FaTimesCircle;

  return (
    <Accordion
      sx={{
        border: '1px solid rgba(15, 23, 42, 0.12)',
        borderRadius: '12px !important',
        '&:before': { display: 'none' },
        mb: 2,
        boxShadow: 'none',
      }}
    >
      <AccordionSummary
        expandIcon={<FaChevronDown />}
        sx={{
          '& .MuiAccordionSummary-content': {
            my: 1.5,
          },
        }}
      >
        <Stack direction="row" spacing={2} alignItems="center" sx={{ width: '100%' }}>
          <Chip
            label={problem.problem_index}
            size="small"
            sx={{
              fontWeight: 600,
              bgcolor: '#4338ca',
              color: '#fff',
              minWidth: 40,
            }}
          />
          <Stack sx={{ flex: 1 }}>
            <Typography variant="subtitle1" sx={{ fontWeight: 600 }}>
              {problem.problem_name}
            </Typography>
            <Typography variant="caption" sx={{ color: 'text.secondary' }}>
              {problem.problem_extra}
            </Typography>
          </Stack>
          <Chip
            icon={<VerdictIcon style={{ fontSize: '14px' }} />}
            label={problem.verdict === 'accepted' ? 'Accepted' : problem.verdict === 'not_accepted' ? 'Wrong Answer' : problem.status || 'Unknown'}
            color={verdictColor}
            size="small"
            sx={{ fontWeight: 600 }}
          />
        </Stack>
      </AccordionSummary>
      <AccordionDetails>
        <Stack spacing={2}>
          <Box>
            <Typography variant="body2" sx={{ mb: 1, color: 'text.secondary' }}>
              Problem Link:
            </Typography>
            <MuiLink
              href={problem.problem_link}
              target="_blank"
              rel="noopener noreferrer"
              sx={{
                display: 'inline-flex',
                alignItems: 'center',
                gap: 0.5,
                color: '#4338ca',
                textDecoration: 'none',
                '&:hover': { textDecoration: 'underline' },
              }}
            >
              View on Codeforces <FaExternalLinkAlt style={{ fontSize: '12px' }} />
            </MuiLink>
          </Box>

          <Box>
            <Typography variant="body2" sx={{ mb: 1, fontWeight: 600 }}>
              Model: {modelName} ({modelProvider})
            </Typography>
          </Box>

          <Box>
            <Typography variant="body2" sx={{ mb: 1, fontWeight: 600 }}>
              Model Output:
            </Typography>
            {loading && <CircularProgress size={20} />}
            {error && <Alert severity="error">Failed to load code: {error.message}</Alert>}
            {data?.data?.code && (
              <Paper
                sx={{
                  p: 2,
                  bgcolor: '#1e293b',
                  color: '#f8fafc',
                  borderRadius: 2,
                  overflow: 'auto',
                  maxHeight: 400,
                }}
              >
                <pre style={{ margin: 0, fontFamily: 'monospace', fontSize: '13px', whiteSpace: 'pre-wrap' }}>
                  {data.data.code}
                </pre>
              </Paper>
            )}
            {!loading && !error && !data?.data?.code && (
              <Typography variant="body2" sx={{ color: 'text.secondary', fontStyle: 'italic' }}>
                No code available for this submission
              </Typography>
            )}
          </Box>

          {data?.data?.verdict && (
            <Box>
              <Typography variant="body2" sx={{ color: 'text.secondary' }}>
                Verdict: <strong>{data.data.verdict}</strong>
              </Typography>
            </Box>
          )}
        </Stack>
      </AccordionDetails>
    </Accordion>
  );
};

export const ExampleQuestions: React.FC = () => {
  const [difficulty, setDifficulty] = useState<ProblemDifficulty>('medium');
  const [benchMode, setBenchMode] = useState<UiBenchmarkMode>('live');

  // Fetch top models by difficulty to select the best one
  const { data: difficultyData, loading: difficultyLoading } = useRequest(
    async () => {
      const apiMode = resolveApiBenchmarkMode(benchMode);
      const problemSet = resolveProblemSet(benchMode);
      return await listLlmByDifficultyLeaderboardLlmDifficultyGet({
        query: {
          difficulty,
          benchmark_mode: apiMode,
          static_problemset: problemSet,
        },
      });
    },
    {
      refreshDeps: [difficulty, benchMode],
    }
  );

  // Select the top model
  const topModel = useMemo(() => {
    const models = difficultyData?.data?.llms?.filter((m) => m.status === 'active') || [];
    const sorted = models.sort((a, b) => (b.passrate || 0) - (a.passrate || 0));
    return sorted[0];
  }, [difficultyData]);

  // Fetch submissions for the top model
  const { data: submissionsData, loading: submissionsLoading } = useRequest(
    async () => {
      if (!topModel) return null;
      const apiMode = resolveApiBenchmarkMode(benchMode);
      const problemSet = resolveProblemSet(benchMode);
      return await getLlmSubmissionsLeaderboardLlmSubmissionsGet({
        query: {
          model_name: topModel.name,
          model_provider: topModel.provider,
          difficulty,
          benchmark_mode: apiMode,
          problemset_name: problemSet,
        },
      });
    },
    {
      ready: !!topModel,
      refreshDeps: [topModel, difficulty, benchMode],
    }
  );

  // Get example problems (first 5 accepted submissions)
  const exampleProblems = useMemo(() => {
    if (!submissionsData?.data?.contests) return [];

    const problems: LlmContestProblemSubmission[] = [];
    for (const contest of submissionsData.data.contests) {
      for (const problem of contest.problems) {
        if (problem.submission_id && (problem.verdict === 'accepted' || problem.verdict === 'not_accepted')) {
          problems.push(problem);
        }
        if (problems.length >= 5) break;
      }
      if (problems.length >= 5) break;
    }
    return problems;
  }, [submissionsData]);

  const isLoading = difficultyLoading || submissionsLoading;

  return (
    <Stack spacing={3}>
      <Paper sx={{ p: { xs: 2, md: 3 } }}>
        <Stack spacing={3}>
          <Stack
            direction={{ xs: 'column', md: 'row' }}
            spacing={2}
            alignItems={{ xs: 'flex-start', md: 'center' }}
            justifyContent="space-between"
          >
            <Stack spacing={0.5}>
              <Typography variant="h6" sx={{ fontWeight: 600 }}>
                Example Questions
              </Typography>
              <Typography variant="body2" sx={{ color: 'text.secondary' }}>
                View sample coding problems with model solutions and accuracy
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
                onChange={(_, value: ProblemDifficulty | null) => {
                  if (value) setDifficulty(value);
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
                <ToggleButton value="easy">Easy</ToggleButton>
                <ToggleButton value="medium">Medium</ToggleButton>
                <ToggleButton value="hard">Hard</ToggleButton>
              </ToggleButtonGroup>
              <ToggleButtonGroup
                value={benchMode}
                exclusive
                onChange={(_, value: UiBenchmarkMode | null) => {
                  if (value) setBenchMode(value);
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
                {benchModeOptions.map((option) => (
                  <ToggleButton key={option.value} value={option.value}>
                    {option.label}
                  </ToggleButton>
                ))}
              </ToggleButtonGroup>
            </Stack>
          </Stack>

          {isLoading && (
            <Box sx={{ display: 'flex', justifyContent: 'center', py: 4 }}>
              <CircularProgress />
            </Box>
          )}

          {!isLoading && topModel && (
            <Box>
              <Typography variant="body2" sx={{ color: 'text.secondary', mb: 2 }}>
                Showing examples from: <strong>{topModel.display_name || topModel.name}</strong> (
                {topModel.provider}) - Pass@1: <strong>{((topModel.passrate || 0) * 100).toFixed(1)}%</strong>
              </Typography>
            </Box>
          )}

          {!isLoading && exampleProblems.length === 0 && (
            <Alert severity="info">No example submissions available for this configuration.</Alert>
          )}

          {!isLoading && topModel && exampleProblems.length > 0 && (
            <Stack spacing={0}>
              {exampleProblems.map((problem) => (
                <ExampleSubmission
                  key={`${problem.problem_link}-${problem.submission_id}`}
                  problem={problem}
                  modelName={topModel.display_name || topModel.name}
                  modelProvider={topModel.provider}
                />
              ))}
            </Stack>
          )}
        </Stack>
      </Paper>
    </Stack>
  );
};
