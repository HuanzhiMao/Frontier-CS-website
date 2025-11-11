import { useRequest } from "ahooks";
import { getSubmissionLeaderboardSubmissionSubmissionIdGet } from "../api";
import { Alert, CircularProgress, Stack, Typography } from "@mui/material";

const style = {
  position: 'absolute',
  top: '50%',
  left: '50%',
  transform: 'translate(-50%, -50%)',
  bgcolor: 'background.paper',
  border: '2px solid #000',
  maxWidth: '75vw',
  maxHeight: '75vh',
  overflow: 'auto',
  boxShadow: 24,
  p: 4,
};

export const Submission: React.FC<{submissionId: string}> = ({submissionId}) => {
  const {data, error, loading} = useRequest(async () => {
    return await getSubmissionLeaderboardSubmissionSubmissionIdGet({
      path: {
        submission_id: submissionId,
      }
    })
  });
  return <Stack spacing={2} sx={style}>
    <Typography variant="h4">Code</Typography>
    {loading && <CircularProgress />}
    {error && <Alert severity="error">{error.message}</Alert>}
    {data?.data && <>
      <pre>{data.data.code}</pre>
    </>}
  </Stack>
}