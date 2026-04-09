function normalizeInlineMathInsideTeX(markdown: string) {
	let next = markdown;
	let previous = '';

	while (next !== previous) {
		previous = next;
		next = next
			.replace(/\\tag\{\$([^$]+)\$\}/g, '\\tag{$1}')
			.replace(/\\text\{([^{}]*)\$([^$]+)\$([^{}]*)\}/g, '\\text{$1$2$3}');
	}

	return next;
}

function normalizeDisplayMathBlocks(markdown: string) {
	const lines = markdown.split(/\r?\n/);
	const output: string[] = [];
	let activeIndent: string | null = null;
	let blockLines: string[] = [];

	for (const line of lines) {
		if (activeIndent === null) {
			const startMatch = line.match(/^(\s*)\$\$(.*)$/);
			if (!startMatch) {
				output.push(line);
				continue;
			}

			const [, indent, remainder] = startMatch;
			const closingIndex = remainder.lastIndexOf('$$');

			if (closingIndex !== -1) {
				const content = remainder.slice(0, closingIndex).trim();
				const trailing = remainder.slice(closingIndex + 2);
				if (trailing.trim()) {
					output.push(line);
					continue;
				}

				output.push(`${indent}$$`);
				if (content) {
					output.push(`${indent}${content}`);
				}
				output.push(`${indent}$$`);
				continue;
			}

			activeIndent = indent;
			output.push(`${indent}$$`);
			const firstLine = remainder.trim();
			if (firstLine) {
				blockLines.push(`${indent}${firstLine}`);
			}
			continue;
		}

		const closeMatch = line.match(/^(.*)\$\$(\s*)$/);
		if (!closeMatch) {
			blockLines.push(line);
			continue;
		}

		const beforeClose = closeMatch[1];
		if (beforeClose.trim()) {
			blockLines.push(beforeClose);
		}

		output.push(...blockLines);
		output.push(`${activeIndent}$$`);
		activeIndent = null;
		blockLines = [];
	}

	if (activeIndent !== null) {
		output.push(...blockLines);
		output.push(`${activeIndent}$$`);
	}

	return output.join('\n');
}

const legacyMathEnvironments = new Set([
	'align',
	'align*',
	'aligned',
	'cases',
	'equation',
	'equation*',
	'gather',
	'gather*',
	'matrix',
	'pmatrix',
	'bmatrix',
	'Bmatrix',
	'vmatrix',
	'Vmatrix',
	'multline',
	'multline*',
	'split',
]);

function normalizeLegacyDisplayMath(markdown: string) {
	const lines = markdown.split(/\r?\n/);
	const output: string[] = [];
	let insideDollarBlock = false;
	let injectedIndent = '';
	let injectedEnvironment: string | null = null;
	let injectedBracketBlock = false;

	for (const line of lines) {
		const trimmed = line.trim();

		if (trimmed === '$$') {
			insideDollarBlock = !insideDollarBlock;
			output.push(line);
			continue;
		}

		if (injectedBracketBlock) {
			if (trimmed === '\\]') {
				output.push(`${injectedIndent}$$`);
				injectedBracketBlock = false;
				injectedIndent = '';
				continue;
			}

			output.push(line);
			continue;
		}

		if (injectedEnvironment !== null) {
			output.push(line);
			if (trimmed === `\\end{${injectedEnvironment}}`) {
				output.push(`${injectedIndent}$$`);
				injectedEnvironment = null;
				injectedIndent = '';
			}
			continue;
		}

		if (insideDollarBlock) {
			output.push(line);
			continue;
		}

		if (trimmed === '\\[') {
			injectedIndent = line.match(/^\s*/)?.[0] ?? '';
			output.push(`${injectedIndent}$$`);
			injectedBracketBlock = true;
			continue;
		}

		const beginMatch = trimmed.match(/^\\begin\{([A-Za-z*]+)\}$/);
		if (beginMatch && legacyMathEnvironments.has(beginMatch[1])) {
			injectedIndent = line.match(/^\s*/)?.[0] ?? '';
			output.push(`${injectedIndent}$$`);
			output.push(line);
			injectedEnvironment = beginMatch[1];
			continue;
		}

		output.push(line);
	}

	if (injectedBracketBlock || injectedEnvironment !== null) {
		output.push(`${injectedIndent}$$`);
	}

	return output.join('\n');
}

function normalizeInlineMathDelimiters(markdown: string) {
	return markdown.replace(/\\\((.+?)\\\)/g, '$$$1$$');
}

function normalizeLegacyTextCommands(markdown: string) {
	const accentMap = new Map<string, string>([
		['\\"A', 'Ae'],
		['\\"E', 'Ee'],
		['\\"I', 'Ie'],
		['\\"O', 'Oe'],
		['\\"U', 'Ue'],
		['\\"a', 'ae'],
		['\\"e', 'ee'],
		['\\"i', 'ie'],
		['\\"o', 'oe'],
		['\\"u', 'ue'],
	]);
	let next = markdown;
	let previous = '';

	while (next !== previous) {
		previous = next;
		next = next
			.replace(/\\emph\{([^{}]+)\}/g, '*$1*')
			.replace(/\\textbf\{([^{}]+)\}/g, '**$1**')
			.replace(/\\textit\{([^{}]+)\}/g, '*$1*')
			.replace(/\{\\em\s+([^{}]+)\}/g, '*$1*');
	}

	for (const [latex, plainText] of accentMap) {
		next = next.replaceAll(latex, plainText);
	}

	return next;
}

function normalizeLegacyLists(markdown: string) {
	const lines = markdown.split(/\r?\n/);
	const output: string[] = [];
	let activeList: 'enumerate' | 'itemize' | null = null;

	for (const line of lines) {
		const trimmed = line.trim();

		if (trimmed === '\\begin{enumerate}') {
			if (output.at(-1)?.trim()) {
				output.push('');
			}
			activeList = 'enumerate';
			continue;
		}

		if (trimmed === '\\begin{itemize}') {
			if (output.at(-1)?.trim()) {
				output.push('');
			}
			activeList = 'itemize';
			continue;
		}

		if (trimmed === '\\end{enumerate}' || trimmed === '\\end{itemize}') {
			if (output.at(-1)?.trim()) {
				output.push('');
			}
			activeList = null;
			continue;
		}

		const itemMatch = line.match(/^\s*\\item\s+(.*)$/);
		if (activeList !== null && itemMatch) {
			output.push(`${activeList === 'enumerate' ? '1.' : '-'} ${itemMatch[1]}`);
			continue;
		}

		output.push(line);
	}

	return output.join('\n');
}

export function normalizeEulerMarkdown(markdown: string) {
	return normalizeLegacyLists(
		normalizeLegacyTextCommands(
			normalizeInlineMathDelimiters(
				normalizeLegacyDisplayMath(normalizeInlineMathInsideTeX(normalizeDisplayMathBlocks(markdown))),
			),
		),
	);
}

export function stripEulerProblemStatementSection(markdown: string) {
	return markdown
		.replace(/^## Problem Statement\s*$[\s\S]*?(?=^##\s)/m, '')
		.replace(/^## Problem Statement\s*$[\s\S]*$/m, '')
		.replace(/\n{3,}/g, '\n\n')
		.trim();
}
