export interface ParsedStatementSample {
	label: string;
	input: string;
	output: string;
	explanationHtml?: string;
}

export interface ParsedStatementSectionBlock {
	type: 'section';
	id: string;
	title: string;
	html: string;
}

export interface ParsedStatementSamplesBlock {
	type: 'samples';
	id: string;
	title: string;
	items: ParsedStatementSample[];
}

export type ParsedStatementBlock = ParsedStatementSectionBlock | ParsedStatementSamplesBlock;

export interface ParsedStatementDocument {
	meta: {
		timeLimit?: string;
		inputFile?: string;
		outputFile?: string;
	};
	introHtml: string;
	blocks: ParsedStatementBlock[];
	hasContent: boolean;
}

const STANDARD_SECTION_PATTERN =
	/^(input|output|interaction|constraints|subtasks|scoring|note|notes|input specification|output specification|explanation|output for the sample input)$/i;
const SAMPLE_INPUT_PATTERN = /^Sample Input(?:\s+(\d+))?$/i;
const SAMPLE_OUTPUT_PATTERN = /^Sample Output(?:\s+(\d+))?$/i;
const SAMPLE_EXPLANATION_PATTERN = /^Explanation of Sample(?:\s+(\d+))?\s*:?\s*(.*)$/i;
const SIDE_BY_SIDE_OUTPUT_PATTERN = /Sample Output(?:\s+(\d+))?|Output for the Sample Input/i;
const SECTION_LABELS = new Map([
	['input', 'Input'],
	['output', 'Output'],
	['interaction', 'Interaction'],
	['constraints', 'Constraints'],
	['subtasks', 'Subtasks'],
	['scoring', 'Scoring'],
	['note', 'Note'],
	['notes', 'Notes'],
	['input specification', 'Input'],
	['output specification', 'Output'],
	['explanation', 'Explanation'],
	['output for the sample input', 'Output for the Sample Input'],
]);

function escapeHtml(value: string) {
	return value
		.replaceAll('&', '&amp;')
		.replaceAll('<', '&lt;')
		.replaceAll('>', '&gt;')
		.replaceAll('"', '&quot;');
}

function normalizeKey(value: string) {
	return value.trim().toLowerCase().replace(/\s+/g, ' ');
}

function slugify(value: string) {
	return value
		.trim()
		.toLowerCase()
		.replace(/[^a-z0-9]+/g, '-')
		.replace(/^-|-$/g, '');
}

function normalizeDisplayText(value: string) {
	return value.trim().replace(/\s+/g, ' ');
}

function normalizeCodeLines(lines: string[]) {
	const trimmed = [...lines];

	while (trimmed.length && !trimmed[0]?.trim()) {
		trimmed.shift();
	}

	while (trimmed.length && !trimmed.at(-1)?.trim()) {
		trimmed.pop();
	}

	return trimmed.map((line) => line.replace(/\s+$/g, '')).join('\n');
}

function trimSharedIndent(lines: string[]) {
	const nonEmpty = lines.filter((line) => line.trim());
	if (!nonEmpty.length) {
		return lines;
	}

	const indent = Math.min(
		...nonEmpty.map((line) => {
			const match = line.match(/^\s*/);
			return match?.[0]?.length ?? 0;
		}),
	);

	return lines.map((line) => line.slice(indent));
}

function isNoiseLine(line: string) {
	const trimmed = line.trim();
	if (!trimmed) {
		return false;
	}

	if (/^\d+(?:st|nd|rd|th)\s+ICPC\b/i.test(trimmed)) {
		return true;
	}

	if (/ICPC Foundation/i.test(trimmed)) {
		return true;
	}

	if (/^This page is intentionally left blank\.?$/i.test(trimmed)) {
		return true;
	}

	if (/^[A-Z]$/.test(trimmed)) {
		return true;
	}

	if (/^[A-Z](?:\s+[A-Z])+$/.test(trimmed)) {
		return true;
	}

	if (/^[a-z]{1,5}$/i.test(trimmed) && /^[ \t]{12,}\S/.test(line)) {
		return true;
	}

	return false;
}

function cleanStatementLines(rawText: string) {
	return rawText
		.replace(/\r\n/g, '\n')
		.replace(/\u000c/g, '\n')
		.split('\n')
		.map((line) => line.replace(/\u00a0/g, ' ').replace(/\s+$/g, ''))
		.filter((line) => !isNoiseLine(line));
}

function stripLeadingHeader(lines: string[], title?: string) {
	const meta: ParsedStatementDocument['meta'] = {};
	const normalizedTitle = title ? normalizeDisplayText(title).toLowerCase() : '';
	let index = 0;

	while (index < lines.length) {
		const line = lines[index]?.trim() ?? '';
		if (!line) {
			index += 1;
			continue;
		}

		const normalizedLine = normalizeDisplayText(line).toLowerCase();
		const timeLimitMatch = line.match(/^Time\s+limit:\s*(.+)$/i);
		if (timeLimitMatch) {
			meta.timeLimit = normalizeDisplayText(timeLimitMatch[1] ?? '');
			index += 1;
			continue;
		}

		const inputFileMatch = line.match(/^Input\s+file:\s*(.+)$/i);
		if (inputFileMatch) {
			meta.inputFile = normalizeDisplayText(inputFileMatch[1] ?? '');
			index += 1;
			continue;
		}

		const outputFileMatch = line.match(/^Output\s+file:\s*(.+)$/i);
		if (outputFileMatch) {
			meta.outputFile = normalizeDisplayText(outputFileMatch[1] ?? '');
			index += 1;
			continue;
		}

		if (/^Problem\b/i.test(line)) {
			index += 1;
			continue;
		}

		if (normalizedTitle && normalizedLine === normalizedTitle) {
			index += 1;
			continue;
		}

		if (index <= 8 && line.length <= 80 && !/[.?!,;:]$/.test(line) && line.split(/\s+/).length <= 8) {
			index += 1;
			continue;
		}

		break;
	}

	return {
		meta,
		lines: lines.slice(index),
	};
}

function isSectionHeading(line: string) {
	return STANDARD_SECTION_PATTERN.test(line.trim());
}

function isSampleInputHeading(line: string) {
	return SAMPLE_INPUT_PATTERN.test(line.trim());
}

function isSampleOutputHeading(line: string) {
	return SAMPLE_OUTPUT_PATTERN.test(line.trim());
}

function hasSideBySideSampleHeadings(line: string) {
	const trimmed = line.trim();
	const inputMatch = /Sample Input(?:\s+\d+)?/i.exec(trimmed);
	const outputMatch = SIDE_BY_SIDE_OUTPUT_PATTERN.exec(trimmed);

	return Boolean(inputMatch && outputMatch && (outputMatch.index ?? 0) > (inputMatch.index ?? 0));
}

function isSampleExplanationHeading(line: string) {
	return SAMPLE_EXPLANATION_PATTERN.test(line.trim());
}

function isBoundaryLine(line: string) {
	const trimmed = line.trim();
	if (!trimmed) {
		return false;
	}

	return (
		isSectionHeading(trimmed) ||
		isSampleInputHeading(trimmed) ||
		hasSideBySideSampleHeadings(trimmed) ||
		isSampleExplanationHeading(trimmed)
	);
}

function splitIntoParagraphBlocks(lines: string[]) {
	const blocks: string[][] = [];
	let current: string[] = [];

	for (const line of lines) {
		if (!line.trim()) {
			if (current.length) {
				blocks.push(current);
				current = [];
			}
			continue;
		}

		current.push(line);
	}

	if (current.length) {
		blocks.push(current);
	}

	return blocks;
}

function renderListBlock(lines: string[]) {
	const items: string[] = [];
	let current = '';
	let ordered = true;

	for (const line of lines) {
		const trimmed = line.trim();
		const bulletMatch = trimmed.match(/^([*\-•]|\d+[.)])\s+(.*)$/);

		if (bulletMatch) {
			if (current) {
				items.push(current);
			}

			ordered = ordered && /^\d/.test(bulletMatch[1] ?? '');
			current = bulletMatch[2]?.trim() ?? '';
			continue;
		}

		if (!current) {
			return '';
		}

		current = `${current} ${trimmed}`.trim();
	}

	if (current) {
		items.push(current);
	}

	if (!items.length) {
		return '';
	}

	const tag = ordered ? 'ol' : 'ul';
	return `<${tag}>${items.map((item) => `<li>${escapeHtml(item)}</li>`).join('')}</${tag}>`;
}

function isListItemLine(line: string) {
	return /^([*\-•]|\d+[.)])\s+/.test(line.trim());
}

function shouldRenderAsPreformatted(lines: string[]) {
	if (!lines.length) {
		return false;
	}

	const indentedLines = lines.filter((line) => /^[ \t]{6,}\S/.test(line)).length;
	const spacedLines = lines.filter((line) => / {3,}\S/.test(line) || /\S {3,}\S/.test(line)).length;
	const allShort = lines.every((line) => line.length <= 120);

	return allShort && (indentedLines > 0 || (lines.length > 1 && spacedLines >= Math.ceil(lines.length / 2)));
}

function renderTextBlocks(lines: string[]) {
	const blocks = splitIntoParagraphBlocks(lines);

	return blocks
		.flatMap((block) => {
			const firstListIndex = block.findIndex((line) => isListItemLine(line));
			if (firstListIndex > 0) {
				return [block.slice(0, firstListIndex), block.slice(firstListIndex)];
			}

			return [block];
		})
		.map((block) => {
			const listHtml = renderListBlock(block);
			if (listHtml) {
				return listHtml;
			}

			if (shouldRenderAsPreformatted(block)) {
				return `<pre class="cp-statement__diagram"><code>${escapeHtml(
					trimSharedIndent(block).join('\n'),
				)}</code></pre>`;
			}

			const paragraph = block.map((line) => line.trim()).join(' ').replace(/\s+/g, ' ').trim();
			if (!paragraph) {
				return '';
			}

			const paragraphClass = /^Figure\b/i.test(paragraph) ? ' class="cp-statement__caption"' : '';
			return `<p${paragraphClass}>${escapeHtml(paragraph)}</p>`;
		})
		.filter(Boolean)
		.join('\n');
}

function collectUntilBoundary(lines: string[], startIndex: number) {
	const collected: string[] = [];
	let index = startIndex;

	while (index < lines.length) {
		const line = lines[index] ?? '';
		if (collected.length && isBoundaryLine(line)) {
			break;
		}

		collected.push(line);
		index += 1;
	}

	return {
		lines: collected,
		nextIndex: index,
	};
}

function parseExplanation(lines: string[], startIndex: number) {
	const match = lines[startIndex]?.trim().match(SAMPLE_EXPLANATION_PATTERN);
	if (!match) {
		return undefined;
	}

	const collected = match[2] ? [match[2]] : [];
	let index = startIndex + 1;

	while (index < lines.length) {
		const line = lines[index] ?? '';
		if (line.trim() && isBoundaryLine(line)) {
			break;
		}

		collected.push(line);
		index += 1;
	}

	return {
		number: match[1],
		html: renderTextBlocks(collected),
		nextIndex: index,
	};
}

function pushSampleBlock(blocks: ParsedStatementBlock[], sample: ParsedStatementSample) {
	const previous = blocks.at(-1);
	if (previous?.type === 'samples') {
		previous.items.push(sample);
		return;
	}

	blocks.push({
		type: 'samples',
		id: `statement-samples-${blocks.filter((block) => block.type === 'samples').length + 1}`,
		title: 'Sample Tests',
		items: [sample],
	});
}

function parseSideBySideSample(lines: string[], startIndex: number) {
	const header = lines[startIndex] ?? '';
	const inputMatch = /Sample Input(?:\s+(\d+))?/i.exec(header);
	const outputMatch = SIDE_BY_SIDE_OUTPUT_PATTERN.exec(header);

	if (!inputMatch || !outputMatch) {
		return undefined;
	}

	const splitIndex = outputMatch.index ?? Math.ceil(header.length / 2);
	const leftLines: string[] = [];
	const rightLines: string[] = [];
	let index = startIndex + 1;

	while (index < lines.length) {
		const line = lines[index] ?? '';
		if (line.trim() && isBoundaryLine(line)) {
			break;
		}

		const left = splitIndex < line.length ? line.slice(0, splitIndex) : line;
		const right = splitIndex < line.length ? line.slice(splitIndex) : '';
		leftLines.push(left.replace(/\s+$/g, ''));
		rightLines.push(right.replace(/^\s+/g, ''));
		index += 1;
	}

	const sampleNumber = inputMatch[1] ?? outputMatch[1];
	const sample: ParsedStatementSample = {
		label: sampleNumber ? `Sample ${sampleNumber}` : 'Sample',
		input: normalizeCodeLines(leftLines),
		output: normalizeCodeLines(rightLines),
	};

	const explanation = parseExplanation(lines, index);
	if (explanation && (!explanation.number || explanation.number === sampleNumber)) {
		sample.explanationHtml = explanation.html;
		index = explanation.nextIndex;
	}

	return {
		sample,
		nextIndex: index,
	};
}

function parseStackedSample(lines: string[], startIndex: number) {
	const inputMatch = lines[startIndex]?.trim().match(SAMPLE_INPUT_PATTERN);
	if (!inputMatch) {
		return undefined;
	}

	let index = startIndex + 1;
	const inputLines: string[] = [];

	while (index < lines.length) {
		const line = lines[index] ?? '';
		if (line.trim() && (isSampleOutputHeading(line) || isBoundaryLine(line))) {
			break;
		}

		inputLines.push(line);
		index += 1;
	}

	const outputLines: string[] = [];
	if (index < lines.length && isSampleOutputHeading(lines[index] ?? '')) {
		index += 1;
		while (index < lines.length) {
			const line = lines[index] ?? '';
			if (line.trim() && isBoundaryLine(line)) {
				break;
			}

			outputLines.push(line);
			index += 1;
		}
	}

	const sampleNumber = inputMatch[1];
	const sample: ParsedStatementSample = {
		label: sampleNumber ? `Sample ${sampleNumber}` : 'Sample',
		input: normalizeCodeLines(inputLines),
		output: normalizeCodeLines(outputLines),
	};

	const explanation = parseExplanation(lines, index);
	if (explanation && (!explanation.number || explanation.number === sampleNumber)) {
		sample.explanationHtml = explanation.html;
		index = explanation.nextIndex;
	}

	return {
		sample,
		nextIndex: index,
	};
}

function parseSection(lines: string[], startIndex: number) {
	const rawTitle = lines[startIndex]?.trim() ?? '';
	const key = normalizeKey(rawTitle);
	const title = SECTION_LABELS.get(key) ?? rawTitle;
	const { lines: bodyLines, nextIndex } = collectUntilBoundary(lines, startIndex + 1);

	return {
		block: {
			type: 'section',
			id: `statement-${slugify(title) || 'section'}`,
			title,
			html: renderTextBlocks(bodyLines),
		} satisfies ParsedStatementSectionBlock,
		nextIndex,
	};
}

export function parseStatementText(
	rawText: string,
	options: {
		title?: string;
	} = {},
): ParsedStatementDocument {
	const cleanedLines = cleanStatementLines(rawText);
	const { meta, lines } = stripLeadingHeader(cleanedLines, options.title);
	const blocks: ParsedStatementBlock[] = [];
	const introLines: string[] = [];
	let index = 0;

	while (index < lines.length) {
		const line = lines[index] ?? '';
		if (!line.trim()) {
			if (!blocks.length) {
				introLines.push(line);
			}
			index += 1;
			continue;
		}

		if (hasSideBySideSampleHeadings(line)) {
			const parsed = parseSideBySideSample(lines, index);
			if (parsed) {
				pushSampleBlock(blocks, parsed.sample);
				index = parsed.nextIndex;
				continue;
			}
		}

		if (isSampleInputHeading(line)) {
			const parsed = parseStackedSample(lines, index);
			if (parsed) {
				pushSampleBlock(blocks, parsed.sample);
				index = parsed.nextIndex;
				continue;
			}
		}

		if (isSampleExplanationHeading(line)) {
			const explanation = parseExplanation(lines, index);
			const lastBlock = blocks.at(-1);
			const lastSample = lastBlock?.type === 'samples' ? lastBlock.items.at(-1) : undefined;
			if (explanation && lastSample) {
				lastSample.explanationHtml = explanation.html;
				index = explanation.nextIndex;
				continue;
			}
		}

		if (isSectionHeading(line)) {
			const section = parseSection(lines, index);
			blocks.push(section.block);
			index = section.nextIndex;
			continue;
		}

		const chunk = collectUntilBoundary(lines, index);
		if (!blocks.length) {
			introLines.push(...chunk.lines);
		} else {
			blocks.push({
				type: 'section',
				id: `statement-notes-${blocks.length + 1}`,
				title: 'Notes',
				html: renderTextBlocks(chunk.lines),
			});
		}
		index = chunk.nextIndex;
	}

	const introHtml = renderTextBlocks(introLines);

	return {
		meta,
		introHtml,
		blocks,
		hasContent: Boolean(
			introHtml ||
				blocks.some((block) => (block.type === 'samples' ? block.items.length > 0 : Boolean(block.html))),
		),
	};
}
