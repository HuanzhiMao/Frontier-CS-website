import { blogPosts } from '../blog/config';

export interface OverviewEntry {
  key: string;
  label: string;
  subtitle: string;
  title: string;
  description: string;
  path: string;
  dateValue: number;
}

export interface QuickNavigationItem {
  key: string;
  title: string;
  anchor: string;
  label: OverviewEntry['label'];
  path: string;
}

const parseDate = (value?: string): number => {
  if (!value) return 0;
  const parsed = Date.parse(value);
  return Number.isNaN(parsed) ? 0 : parsed;
};

export const buildOverviewEntries = (): OverviewEntry[] => {
  const formatter = new Intl.DateTimeFormat('en-US', {
    month: 'short',
    day: 'numeric',
    year: 'numeric',
  });

  const blogEntries: OverviewEntry[] = blogPosts.map((post) => {
    const dateValue = parseDate(post.publishedAt);
    const subtitle = post.displayDate
      ? post.displayDate
      : dateValue
      ? formatter.format(dateValue)
      : post.publishedAt;

    return {
      key: post.slug,
      label: 'Release',
      subtitle,
      title: post.title,
      description: post.summary,
      path: post.path,
      dateValue,
    };
  });

  return [...blogEntries].sort((a, b) => b.dateValue - a.dateValue);
};

export const buildQuickNavigationItems = (): QuickNavigationItem[] =>
  buildOverviewEntries().map((entry) => ({
    key: entry.key,
    title: entry.title,
    anchor: `overview-${entry.key}`,
    label: entry.label,
    path: entry.path,
  }));
