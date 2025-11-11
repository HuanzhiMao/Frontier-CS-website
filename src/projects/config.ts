export interface ProjectNavLink {
  label: string;
  path: string;
}

export interface ProjectExternalLink {
  label: string;
  href: string;
}

export interface ProjectHeroSection {
  title: string;
  description: string;
  match: (pathname: string) => boolean;
}

export interface ProjectOverviewCard {
  subtitle: string;
  description: string;
  highlights?: string[];
  metrics?: Array<{ label: string; value: string }>;
  actionLabel: string;
  actionPath: string;
  secondaryAction?: { label: string; href: string };
}

export interface ProjectConfig {
  slug: string;
  name: string;
  basePath: string;
  overviewPath: string;
  heroSections: ProjectHeroSection[];
  navigationLinks: ProjectNavLink[];
  externalLinks?: ProjectExternalLink[];
  overviewCard: ProjectOverviewCard;
  headerUtilityLinks?: ProjectNavLink[];
}

const liveCodeBenchProBasePath = '/projects/livecodebench-pro';

export const projects: ProjectConfig[] = [
  {
    slug: 'livecodebench-pro',
    name: 'LiveCodeBench Pro',
    basePath: liveCodeBenchProBasePath,
    overviewPath: `${liveCodeBenchProBasePath}/overview`,
    heroSections: [
      {
        title: 'LiveCodeBench Pro',
        description:
          'We introduce LiveCodeBench Pro, a benchmark composed of problems from Codeforces, ICPC, and IOI that are continuously updated to reduce the likelihood of data contamination.',
        match: (pathname: string) =>
          pathname === liveCodeBenchProBasePath || pathname === `${liveCodeBenchProBasePath}/overview`,
      },
      {
        title: 'LiveCodeBench Pro',
        description:
          'We introduce LiveCodeBench Pro, a benchmark composed of problems from Codeforces, ICPC, and IOI that are continuously updated to reduce the likelihood of data contamination.',
        match: (pathname: string) => pathname.startsWith(`${liveCodeBenchProBasePath}/leaderboard`),
      },
      {
        title: 'LiveCodeBench Pro',
        description: 'Visualise model performance changes over time across benchmark versions.',
        match: (pathname: string) => pathname.startsWith(`${liveCodeBenchProBasePath}/rating-trend`),
      },
      {
        title: 'Submit to the Standard Judge',
        description:
          'Queue a new evaluation by uploading your model\'s run outputs to the LiveCodeBench Pro standard judge.',
        match: (pathname: string) => pathname.startsWith(`${liveCodeBenchProBasePath}/submit`),
      },
    ],
    navigationLinks: [
      { label: 'Overview', path: `${liveCodeBenchProBasePath}/overview` },
      { label: 'Full Leaderboard', path: `${liveCodeBenchProBasePath}/leaderboard` },
      { label: 'Rating Trend', path: `${liveCodeBenchProBasePath}/rating-trend` },
      { label: 'Submit Run', path: `${liveCodeBenchProBasePath}/submit` },
    ],
    externalLinks: [
      { label: 'Technique Report', href: 'https://arxiv.org/abs/2506.11928' },
    ],
    overviewCard: {
      subtitle: 'June 13, 2025',
      description:
        'A benchmark composed of problems from Codeforces, ICPC, and IOI that are continuously updated to reduce the likelihood of data contamination.',
      actionLabel: 'View project',
      actionPath: `${liveCodeBenchProBasePath}/overview`,
      secondaryAction: {
        label: 'Read Technique Report',
        href: 'https://arxiv.org/abs/2506.11928',
      },
    },
  },
  {
    slug: 'autocode',
    name: 'AutoCode',
    basePath: '/projects/autocode',
    overviewPath: '/projects/autocode/overview',
    heroSections: [
      {
        title: 'AutoCode',
        description:
          'We found LLMs could be used as problem setters for competitive programming and could generate test cases for general problems involving input-output matching.',
        match: (pathname: string) =>
          pathname === '/projects/autocode' || pathname === '/projects/autocode/overview',
      },
    ],
    navigationLinks: [{ label: 'Overview', path: '/projects/autocode/overview' }],
    externalLinks: [{ label: 'Technique Report', href: 'https://arxiv.org/abs/2510.12803' }],
    overviewCard: {
      subtitle: 'October 15, 2025',
      description:
        'We found LLMs could be used as problem setters for competitive programming and could generate test cases for general problems involving input-output matching.',
      actionLabel: 'View project',
      actionPath: '/projects/autocode/overview',
    },
  },
];

export const defaultHero = {
  title: 'FrontierCS',
  description:
    'FrontierCS is an expert-curated benchmark of open-ended computer science challenges, blending NP-hard programming variants and research tasks with objective evaluators that evolve as models surpass human baselines.',
};

export const findProjectByPath = (pathname: string): ProjectConfig | undefined =>
  projects.find((project) => pathname.startsWith(project.basePath));
