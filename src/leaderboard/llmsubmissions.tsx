import { Box, Table, TableHead, TableRow, TableCell, TableBody, Paper, CircularProgress, Alert, Typography, Stack, Tooltip, Modal } from "@mui/material"
import { useRequest } from "ahooks"
import { getLlmSubmissionsLeaderboardLlmSubmissionsGet, BenchmarkMode, ProblemDifficulty, listLlmByDifficultyLeaderboardLlmDifficultyGet, LlmByDifficulty, listLlmLeaderboardLlmGet } from "../api"
import styled from "@emotion/styled"
import { Fragment, useState, useMemo } from "react"
import { Submission } from "./submission"

const StatusContainer = styled.div`
  display: flex;
  gap: 8px;
  flex-direction: row;
`

const ProblemID = styled.a`
  padding: 5px;
  font-weight: bold;
  text-align: center;
  line-height: 40px;
  width: 40px;
`

const Correct = styled.div`
  padding: 5px;
  height: 45px;
  min-width: 45px;
  background-color: green;
  color: white;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: space-around;
`

const Incorrect = styled.div`
  padding: 5px;
  height: 45px;
  min-width: 45px;
  background-color: red;
  color: white;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: space-around;
`

const Unknown = styled.div`
  padding: 5px;
  height: 45px;
  min-width: 45px;
  background-color: grey;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: space-around;
`

const InQueue = styled.div`
  padding: 5px;
  height: 45px;
  min-width: 45px;
  background-color: yellow;
  text-color: white;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: space-around;
`

const style = {
  position: 'absolute',
  top: '50%',
  left: '50%',
  transform: 'translate(-50%, -50%)',
  bgcolor: 'background.paper',
  border: '2px solid #000',
  boxShadow: 24,
  p: 4,
};

const tableContainerStyle = {
  maxWidth: '80vw',
  maxHeight: '80vh',
  overflow: 'auto',
}

export const LLMSubmissionsViewer: React.FC<{name: string; provider: string; difficulty: ProblemDifficulty; bench_mode: BenchmarkMode; problemSetName?: string | null}> = ({name, provider, difficulty, bench_mode, problemSetName}) => {
  const {data, error, loading} = useRequest(async () => {
    return await getLlmSubmissionsLeaderboardLlmSubmissionsGet({
      query: {
        model_name: name,
        model_provider: provider,
        difficulty: difficulty,
        benchmark_mode: bench_mode,
        problemset_name: problemSetName,
      }
    })
  });

  const {data: difficultyData, error: difficultyError, loading: difficultyLoading} = useRequest(async () => {
    if (difficulty === 'all') return null; // Don't fetch if difficulty is 'all'
    return await listLlmByDifficultyLeaderboardLlmDifficultyGet({
      query: {
        difficulty: difficulty,
        benchmark_mode: bench_mode,
        static_problemset: problemSetName,
      }
    });
  }, {
    ready: difficulty !== 'all', // Only run if difficulty is not 'all'
  });

  // Fallback: fetch overall LLM list to get display_name when difficulty data is not available (e.g., difficulty === 'all')
  const { data: allLlmsData } = useRequest(listLlmLeaderboardLlmGet);

  const currentModelDifficultyData: LlmByDifficulty | undefined | null = useMemo(() => {
    if (!difficultyData?.data?.llms) return null;
    return difficultyData.data.llms.find(llm => llm.name === name && llm.provider === provider);
  }, [difficultyData, name, provider]);

  const [ open, setOpen ] = useState(false);
  const [ submissionId, setSubmissionId ] = useState<string | null>(null);
  const fallbackLlm = useMemo(() => {
    const list = allLlmsData?.data ?? [];
    return list.find(llm => llm.name === name && llm.provider === provider);
  }, [allLlmsData, name, provider]);
  const displayModelName = (currentModelDifficultyData?.display_name ?? fallbackLlm?.display_name ?? name);
  return <Box sx={style}>
    <Modal open={open} onClose={() => setOpen(false)}>
      <>
        {submissionId && <Submission submissionId={submissionId} />}
      </>
    </Modal>
    <Stack spacing={2}>
      <Typography variant="h4">Submissions for {displayModelName} ({provider})</Typography>
      {difficulty !== 'all' && (difficultyLoading ? <CircularProgress size={20} /> : difficultyError ? <Alert severity="error">{difficultyError.message}</Alert> : currentModelDifficultyData && (
        <Typography variant="h6">
          Evaluation Progress: {(currentModelDifficultyData.validrate * 100).toFixed(0)}%
        </Typography>
      ))}
      {loading && <CircularProgress />}
      {error && <Alert severity="error">{error.message}</Alert>}
      {data?.data && <Paper>
        <div style={tableContainerStyle}>
          <Table>
            <TableHead>
              <TableRow>
                <TableCell>Contest</TableCell>
                <TableCell>Problems (Click on colored box to view the submission detail)</TableCell>
              </TableRow>
            </TableHead>
            <TableBody>
              {data.data.contests.map(contest => {
                return <TableRow key={contest.contest_link}>
                  <TableCell><a href={contest.contest_link} target="_blank">{contest.contest_title}</a></TableCell>
                  <TableCell>
                    <StatusContainer key={contest.contest_link}>
                      {contest.problems.map(problem => {
                        const showModal = () => {
                          setOpen(true);
                          setSubmissionId(problem.submission_id);
                        }
                        return <Fragment key={problem.problem_index}>
                          <Tooltip title={problem.problem_extra}>
                            <ProblemID href={problem.problem_link} target="_blank">{problem.problem_index}</ProblemID>
                          </Tooltip>
						  {(() => {
							  switch(problem.verdict) {
								  case "accepted": 
									  return <Correct onClick={showModal}>{problem.status}</Correct>;
								  case "not_accepted": 
									  return <Incorrect onClick={showModal}>{problem.status}</Incorrect>;
								  case "in_queue": 
									  return <InQueue onClick={showModal} >{problem.status}</InQueue>;
								  default:
									  return <Unknown onClick={showModal} >{problem.status}</Unknown>;
							  }
						  })()}
                        </Fragment>
                      })}
                    </StatusContainer>
                  </TableCell>
                </TableRow>
              })}
            </TableBody>
          </Table>
        </div>
      </Paper>}
    </Stack>
  </Box>
}
