export interface BlogPost {
  slug: string;
  title: string;
  path: string;
  publishedAt: string;
  summary: string;
  heroDescription?: string;
  displayDate?: string;
}

export const blogBasePath = '/release';

export const blogPosts: BlogPost[] = [];

export const findBlogByPath = (pathname: string): BlogPost | undefined =>
  blogPosts.find((post) => pathname.startsWith(post.path));
