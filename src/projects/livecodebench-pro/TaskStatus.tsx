import React, { useCallback, useEffect, useMemo } from 'react';
import {
  Alert,
  Box,
  Button,
  Card,
  CardContent,
  CardHeader,
  Chip,
  CircularProgress,
  Divider,
  Stack,
  Typography,
} from '@mui/material';
import { useRequest } from 'ahooks';
import { useParams, useSearchParams } from 'react-router-dom';

import {
  getStandardJudgeTaskStandardJudgeTaskIdGet,
  GetStandardJudgeTaskResponse,
} from '../../api';

const formatTimestamp = (timestamp?: number | null) => {
  if (!timestamp) return null;
  const date = new Date(timestamp * 1000);
  if (Number.isNaN(date.getTime())) {
    return null;
  }
  return date.toLocaleString();
};

const useErrorMessage = () =>
  useCallback((error: unknown): string => {
    if (!error) return 'Something went wrong. Please try again.';
    if (typeof error === 'string') return error;
    if (error instanceof Error) return error.message;
    const maybeMessage = (error as { message?: unknown })?.message;
    if (typeof maybeMessage === 'string') {
      return maybeMessage;
    }
    const response = (error as { response?: { data?: unknown } }).response;
    const responseData = response?.data as { detail?: unknown } | undefined;
    if (typeof responseData?.detail === 'string') {
      return responseData.detail;
    }
    return 'Something went wrong. Please try again.';
  }, []);

const statusLabel: Record<GetStandardJudgeTaskResponse['status'], string> = {
  pending: 'Pending',
  instance_created: 'Instance created',
  running: 'Running',
  finished: 'Finished',
  failed: 'Failed',
};

const statusColor: Record<GetStandardJudgeTaskResponse['status'], 'default' | 'info' | 'warning' | 'success' | 'error'> = {
  pending: 'default',
  instance_created: 'info',
  running: 'info',
  finished: 'success',
  failed: 'error',
};

const DetailRow: React.FC<{ label: string; value: React.ReactNode }> = ({ label, value }) => (
  <Stack direction={{ xs: 'column', sm: 'row' }} spacing={0.5} alignItems={{ sm: 'baseline' }}>
    <Typography variant="subtitle2" sx={{ minWidth: { sm: 160 }, color: 'text.secondary' }}>
      {label}
    </Typography>
    <Box sx={{ wordBreak: 'break-word' }}>
      {typeof value === 'string' || typeof value === 'number' ? (
        <Typography variant="body2">{value}</Typography>
      ) : (
        value
      )}
    </Box>
  </Stack>
);

export const TaskStatus: React.FC = () => {
  const { taskId } = useParams<{ taskId: string }>();
  const [searchParams] = useSearchParams();
  const secret = searchParams.get('secret') ?? '';

  const getErrorMessage = useErrorMessage();

  const { data, error, loading, run } = useRequest(
    async ({ taskId: requestTaskId, secret: requestSecret }: { taskId: string; secret: string }) => {
      const response = await getStandardJudgeTaskStandardJudgeTaskIdGet<true>({
        path: { task_id: requestTaskId },
        query: { secret: requestSecret },
        throwOnError: true,
      });
      return response.data;
    },
    { manual: true },
  );

  const fetchStatus = useCallback(() => {
    if (!taskId || !secret) return;
    void run({ taskId, secret });
  }, [run, secret, taskId]);

  useEffect(() => {
    fetchStatus();
  }, [fetchStatus]);

  const timeline = useMemo(() => {
    if (!data) return [];
    return [
      { label: 'Task created', value: formatTimestamp(data.created_at) },
      { label: 'Instance created', value: formatTimestamp(data.instance_created_at) },
      { label: 'Execution started', value: formatTimestamp(data.running_at) },
      { label: 'Execution finished', value: formatTimestamp(data.finished_at) },
      { label: 'Failed at', value: formatTimestamp(data.failed_at) },
    ].filter((item) => item.value);
  }, [data]);

  const errorMessage = useMemo(() => (error ? getErrorMessage(error) : null), [error, getErrorMessage]);

  return (
    <Card>
      <CardHeader
        title="Standard Judge Task"
        subheader={taskId ? `Task ID: ${taskId}` : undefined}
        action={
          taskId && secret ? (
            <Button variant="outlined" onClick={fetchStatus} disabled={loading}>
              {loading ? <CircularProgress size={18} /> : 'Refresh'}
            </Button>
          ) : undefined
        }
      />
      <Divider />
      <CardContent>
        <Stack spacing={3}>
          {(!taskId || !secret) && (
            <Alert severity="warning">
              Provide both a task id and `secret` query parameter to view the task status.
            </Alert>
          )}

          {errorMessage && <Alert severity="error">{errorMessage}</Alert>}

          {loading && !data && (
            <Stack alignItems="center" justifyContent="center" sx={{ py: 4 }}>
              <CircularProgress />
            </Stack>
          )}

          {data && (
            <Stack spacing={3}>
              <Stack spacing={1.5}>
                <DetailRow label="Model" value={data.model} />
                <DetailRow label="Author" value={data.author} />
                <DetailRow
                  label="Status"
                  value={<Chip label={statusLabel[data.status]} color={statusColor[data.status]} size="small" />}
                />
                <DetailRow
                  label="Input archive"
                  value={
                    <Button
                      component="a"
                      href={data.input_file_url}
                      target="_blank"
                      rel="noopener"
                      size="small"
                      variant="outlined"
                      download
                    >
                      Download
                    </Button>
                  }
                />
                {data.output_file_url && (
                  <DetailRow
                    label="Output archive"
                    value={
                      <Button
                        component="a"
                        href={data.output_file_url}
                        target="_blank"
                        rel="noopener"
                        size="small"
                        variant="outlined"
                        download
                      >
                        Download
                      </Button>
                    }
                  />
                )}
              </Stack>

              {timeline.length > 0 && (
                <Box>
                  <Typography variant="subtitle2" sx={{ color: 'text.secondary', mb: 1 }}>
                    Timeline
                  </Typography>
                  <Stack spacing={1.25}>
                    {timeline.map((item) => (
                      <DetailRow key={item.label} label={item.label} value={item.value ?? ''} />
                    ))}
                  </Stack>
                </Box>
              )}
            </Stack>
          )}
        </Stack>
      </CardContent>
    </Card>
  );
};

export default TaskStatus;
