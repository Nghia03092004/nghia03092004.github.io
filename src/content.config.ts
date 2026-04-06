import { defineCollection } from 'astro/content/config';
import { glob } from 'astro/loaders';
import { z } from 'astro/zod';

const sharedFields = {
	title: z.string(),
	description: z.string(),
	published: z.coerce.date(),
	updated: z.coerce.date().optional(),
	draft: z.boolean().default(false),
	featured: z.boolean().default(false),
	tags: z.array(z.string()).default([]),
};

const essays = defineCollection({
	loader: glob({
		pattern: '**/*.{md,mdx}',
		base: './src/data/essays',
	}),
	schema: z.object({
		...sharedFields,
		readingTime: z.string().optional(),
	}),
});

const code = defineCollection({
	loader: glob({
		pattern: '**/*.{md,mdx}',
		base: './src/data/code',
	}),
	schema: z.object({
		...sharedFields,
		readingTime: z.string().optional(),
		repository: z.string().optional(),
	}),
});

const euler = defineCollection({
	loader: glob({
		pattern: 'problem_*/solution.md',
		base: './project_euler_unified',
		generateId: ({ entry }) => {
			const match = entry.match(/^problem_(\d+)\/solution\.md$/);
			if (!match) {
				return entry;
			}

			return `problem-${match[1].padStart(4, '0')}`;
		},
	}),
	schema: z.object({}),
});

export const collections = { essays, code, euler };
