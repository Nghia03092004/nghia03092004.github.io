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
		pattern: '**/*.{md,mdx}',
		base: './src/data/project-euler',
	}),
	schema: z.object({
		...sharedFields,
		problem: z.number().int().positive(),
		status: z.enum(['solved', 'working', 'revisit']).default('solved'),
		difficulty: z.enum(['warmup', 'medium', 'hard']).default('medium'),
		languages: z.array(z.string()).default([]),
		answer: z.string().optional(),
	}),
});

export const collections = { essays, code, euler };
