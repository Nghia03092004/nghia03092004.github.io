import { promises as fs } from 'node:fs';
import path from 'node:path';

const EULER_ROOT = path.resolve(process.cwd(), 'project_euler');
const STATEMENT_LICENSE_URL = 'https://projecteuler.net/copyright';
const statementCache = new Map<number, Promise<EulerMirroredStatement | undefined>>();

export interface EulerMirroredStatement {
	html: string;
	sourceUrl: string;
	licenseUrl: string;
}

function getStatementPath(problem: number) {
	return path.join(EULER_ROOT, `problem_${String(problem).padStart(3, '0')}`, 'statement.html');
}

function getProblemUrl(problem: number) {
	return `https://projecteuler.net/problem=${problem}`;
}

export async function getEulerMirroredStatement(problem: number) {
	const cached = statementCache.get(problem);
	if (cached) {
		return cached;
	}

	const promise = fs
		.readFile(getStatementPath(problem), 'utf8')
		.then((html) => html.trim())
		.then((html) =>
			html
				? {
						html,
						sourceUrl: getProblemUrl(problem),
						licenseUrl: STATEMENT_LICENSE_URL,
					}
				: undefined,
		)
		.catch((error: NodeJS.ErrnoException) => {
			if (error.code === 'ENOENT') {
				return undefined;
			}

			throw error;
		});

	statementCache.set(problem, promise);
	return promise;
}
