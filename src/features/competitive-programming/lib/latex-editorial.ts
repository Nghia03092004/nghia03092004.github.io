interface TocHeading {
	depth: number;
	slug: string;
	text: string;
}

interface LatexSection {
	title: string;
	fullSource: string;
	content: string;
}

export interface RenderedLatexContent {
	html: string;
	headings: TocHeading[];
	usedFallback: boolean;
}

export interface ExtractedLatexDocument {
	body: string;
	statementTitle?: string;
	statementSource?: string;
	editorialSource: string;
}

const BLOCK_MATH_ENVIRONMENTS = new Set([
	'align',
	'align*',
	'aligned',
	'alignedat',
	'cases',
	'equation',
	'equation*',
	'gather',
	'gather*',
	'matrix',
	'pmatrix',
	'bmatrix',
	'vmatrix',
	'Vmatrix',
	'multline',
	'multline*',
	'split',
]);

const THEOREM_LIKE_ENVIRONMENTS = new Set([
	'claim',
	'corollary',
	'definition',
	'lemma',
	'observation',
	'proof',
	'theorem',
]);

const STATEMENT_SECTION_PATTERN = /^(problem\s+(statement|summary)|statement)$/i;
const IMPLEMENTATION_SECTION_PATTERN = /^(implementation|implementation details|reference implementation|code)$/i;

function escapeHtml(value: string) {
	return value
		.replaceAll('&', '&amp;')
		.replaceAll('<', '&lt;')
		.replaceAll('>', '&gt;')
		.replaceAll('"', '&quot;');
}

function stripLatexForPlainText(value: string) {
	let text = value
		.replace(/(^|[^\\])%.*$/gm, '$1')
		.replace(/\\label\{[^}]*\}/g, ' ')
		.replace(/\\(?:cite|eqref|autoref)\{[^}]*\}/g, ' ')
		.replace(/\\begin\{[^}]+\}/g, ' ')
		.replace(/\\end\{[^}]+\}/g, ' ')
		.replace(/\\item\b/g, ' ')
		.replace(/\$\$([\s\S]*?)\$\$/g, ' $1 ')
		.replace(/\$([^$\n]+)\$/g, ' $1 ')
		.replace(/\\\[((?:.|\n)*?)\\\]/g, ' $1 ')
		.replace(/\\\(((?:.|\n)*?)\\\)/g, ' $1 ')
		.replace(/\\\\/g, ' ')
		.replace(/~+/g, ' ')
		.replace(/\\(?:qquad|quad|,|;|:|!)/g, ' ');

	for (let index = 0; index < 8; index += 1) {
		text = text.replace(/\\[A-Za-z@*]+(?:\[[^\]]*\])?\{([^{}]*)\}/g, '$1');
	}

	return text
		.replace(/\\[A-Za-z@*]+/g, ' ')
		.replace(/[{}]/g, ' ')
		.replace(/\s+([,.;:!?])/g, '$1')
		.replace(/\(\s+/g, '(')
		.replace(/\s+\)/g, ')')
		.replace(/\s+/g, ' ')
		.trim();
}

function normalizeSlug(value: string) {
	return value
		.trim()
		.toLowerCase()
		.replace(/[_\s]+/g, '-')
		.replace(/[^a-z0-9-]/g, '-')
		.replace(/-+/g, '-')
		.replace(/^-|-$/g, '');
}

function capitalize(value: string) {
	if (!value) {
		return value;
	}

	return value[0].toUpperCase() + value.slice(1);
}

function extractDocumentBody(texSource: string) {
	const bodyStart = texSource.match(/\\begin\{document\}/);
	const bodyEnd = texSource.match(/\\end\{document\}/);
	const startIndex = bodyStart ? (bodyStart.index ?? 0) + bodyStart[0].length : 0;
	const endIndex = bodyEnd?.index ?? texSource.length;

	return texSource
		.slice(startIndex, endIndex)
		.replace(/\r\n/g, '\n')
		.replace(/\\maketitle\b/g, '')
		.replace(/\\(?:smallskip|medskip|bigskip)\b/g, '\n\n')
		.replace(/\\(?:newpage|pagebreak|clearpage|par)\b/g, '\n\n')
		.replace(/\\noindent\b/g, '')
		.trim();
}

function splitTopLevelSections(body: string) {
	const matches = [...body.matchAll(/\\section\*?\{([^}]*)\}/g)];

	if (!matches.length) {
		return {
			preamble: body.trim(),
			sections: [] as LatexSection[],
		};
	}

	const preamble = body.slice(0, matches[0]?.index ?? 0).trim();
	const sections = matches.map((match, index) => {
		const start = match.index ?? 0;
		const contentStart = start + match[0].length;
		const end = matches[index + 1]?.index ?? body.length;

		return {
			title: match[1]?.trim() ?? '',
			fullSource: body.slice(start, end).trim(),
			content: body.slice(contentStart, end).trim(),
		} satisfies LatexSection;
	});

	return { preamble, sections };
}

function isImplementationSection(section: LatexSection) {
	return (
		IMPLEMENTATION_SECTION_PATTERN.test(section.title) &&
		/\\begin\{(?:lstlisting|verbatim)\}|\\lstinputlisting\b/.test(section.content)
	);
}

export function extractLatexDocument(texSource: string): ExtractedLatexDocument {
	const body = extractDocumentBody(texSource);
	const { preamble, sections } = splitTopLevelSections(body);
	const statementSection = sections.find((section) => STATEMENT_SECTION_PATTERN.test(section.title));
	const editorialSections = sections.filter(
		(section) => section !== statementSection && !isImplementationSection(section),
	);

	if (!statementSection) {
		const editorialSource = [preamble, ...editorialSections.map((section) => section.fullSource)]
			.filter(Boolean)
			.join('\n\n')
			.trim();

		return {
			body,
			editorialSource: editorialSource || body,
		};
	}

	const editorialParts = [preamble, ...editorialSections.map((section) => section.fullSource)].filter(Boolean);

	return {
		body,
		statementTitle: statementSection.title,
		statementSource: statementSection.content,
		editorialSource: editorialParts.join('\n\n').trim() || body,
	};
}

function readBalancedGroup(source: string, startIndex: number, opener: string, closer: string) {
	if (source[startIndex] !== opener) {
		return null;
	}

	let index = startIndex + 1;
	let depth = 1;

	while (index < source.length) {
		const character = source[index];
		if (character === '\\') {
			index += 2;
			continue;
		}
		if (character === opener) {
			depth += 1;
			index += 1;
			continue;
		}
		if (character === closer) {
			depth -= 1;
			if (depth === 0) {
				return {
					content: source.slice(startIndex + 1, index),
					nextIndex: index + 1,
				};
			}
		}
		index += 1;
	}

	return null;
}

function splitMathSegments(value: string) {
	const segments: Array<{ type: 'text' | 'math'; value: string }> = [];
	let buffer = '';
	let index = 0;

	const flushBuffer = () => {
		if (buffer) {
			segments.push({ type: 'text', value: buffer });
			buffer = '';
		}
	};

	while (index < value.length) {
		if (value.startsWith('$$', index)) {
			const end = value.indexOf('$$', index + 2);
			if (end !== -1) {
				flushBuffer();
				segments.push({ type: 'math', value: value.slice(index, end + 2) });
				index = end + 2;
				continue;
			}
		}

		if (value.startsWith('\\[', index)) {
			const end = value.indexOf('\\]', index + 2);
			if (end !== -1) {
				flushBuffer();
				segments.push({ type: 'math', value: value.slice(index, end + 2) });
				index = end + 2;
				continue;
			}
		}

		if (value.startsWith('\\(', index)) {
			const end = value.indexOf('\\)', index + 2);
			if (end !== -1) {
				flushBuffer();
				segments.push({ type: 'math', value: value.slice(index, end + 2) });
				index = end + 2;
				continue;
			}
		}

		if (value[index] === '$') {
			let cursor = index + 1;
			while (cursor < value.length) {
				if (value[cursor] === '$' && value[cursor - 1] !== '\\') {
					flushBuffer();
					segments.push({ type: 'math', value: value.slice(index, cursor + 1) });
					index = cursor + 1;
					break;
				}
				cursor += 1;
			}
			if (cursor < value.length) {
				continue;
			}
		}

		buffer += value[index] ?? '';
		index += 1;
	}

	flushBuffer();
	return segments;
}

function readCommandName(source: string, startIndex: number) {
	if (source[startIndex] !== '\\') {
		return null;
	}

	const firstCharacter = source[startIndex + 1];
	if (!firstCharacter) {
		return null;
	}

	if (!/[A-Za-z@]/.test(firstCharacter)) {
		return {
			name: firstCharacter,
			nextIndex: startIndex + 2,
		};
	}

	let index = startIndex + 1;
	while (index < source.length && /[A-Za-z@]/.test(source[index] ?? '')) {
		index += 1;
	}
	if (source[index] === '*') {
		index += 1;
	}

	return {
		name: source.slice(startIndex + 1, index),
		nextIndex: index,
	};
}

function renderInlineText(value: string): string {
	const pieces = splitMathSegments(value).map((segment) => {
		if (segment.type === 'math') {
			return escapeHtml(segment.value);
		}

		return renderTextWithoutMath(segment.value);
	});

	return pieces.join('');
}

function renderTextWithoutMath(value: string): string {
	let html = '';
	let index = 0;

	while (index < value.length) {
		const character = value[index];

		if (character === '\\') {
			if (value.startsWith('\\\\', index)) {
				html += '<br />';
				index += 2;
				continue;
			}

			const command = readCommandName(value, index);
			if (!command) {
				html += escapeHtml(character);
				index += 1;
				continue;
			}

			if ('{}%$&#_'.includes(command.name)) {
				html += escapeHtml(command.name);
				index = command.nextIndex;
				continue;
			}

			if (command.name === ' ') {
				html += ' ';
				index = command.nextIndex;
				continue;
			}

			if (['quad', 'qquad', ',', ';', ':', '!', 'enspace', 'hfill'].includes(command.name)) {
				html += ' ';
				index = command.nextIndex;
				continue;
			}

			if (['LaTeX', 'TeX'].includes(command.name)) {
				html += command.name;
				index = command.nextIndex;
				continue;
			}

			if (['qed', 'qedhere'].includes(command.name)) {
				html += '<span class="cp-editorial__qed">&#9633;</span>';
				index = command.nextIndex;
				continue;
			}

			if (['linebreak', 'newline'].includes(command.name)) {
				html += '<br />';
				index = command.nextIndex;
				continue;
			}

			const inlineGroup = (() => {
				let nextIndex = command.nextIndex;
				while (/\s/.test(value[nextIndex] ?? '')) {
					nextIndex += 1;
				}
				return readBalancedGroup(value, nextIndex, '{', '}');
			})();

			if (inlineGroup) {
				const renderedContent = renderInlineText(inlineGroup.content);
				switch (command.name) {
					case 'emph':
					case 'textit':
					case 'mathit':
						html += `<em>${renderedContent}</em>`;
						break;
					case 'textbf':
						html += `<strong>${renderedContent}</strong>`;
						break;
					case 'texttt':
						html += `<code>${renderedContent}</code>`;
						break;
					case 'underline':
						html += `<span class="cp-editorial__underline">${renderedContent}</span>`;
						break;
					case 'textsc':
						html += `<span class="cp-editorial__smallcaps">${renderedContent}</span>`;
						break;
					case 'text':
					case 'mbox':
					case 'textrm':
					case 'mathrm':
						html += renderedContent;
						break;
					case 'href': {
						const url = inlineGroup.content.trim();
						const labelGroup = readBalancedGroup(value, inlineGroup.nextIndex, '{', '}');
						if (labelGroup) {
							html += `<a href="${escapeHtml(url)}">${renderInlineText(labelGroup.content)}</a>`;
							index = labelGroup.nextIndex;
							continue;
						}
						html += escapeHtml(`\\${command.name}{${inlineGroup.content}}`);
						break;
					}
					case 'url': {
						const url = inlineGroup.content.trim();
						html += `<a href="${escapeHtml(url)}">${escapeHtml(url)}</a>`;
						break;
					}
					default:
						html += renderedContent;
						break;
				}
				index = inlineGroup.nextIndex;
				continue;
			}

			html += escapeHtml(`\\${command.name}`);
			index = command.nextIndex;
			continue;
		}

		if (character === '~') {
			html += ' ';
			index += 1;
			continue;
		}

		html += escapeHtml(character);
		index += 1;
	}

	return html;
}

function renderPlainTextStatement(value: string) {
	return escapeHtml(value.trim());
}

class LatexHtmlRenderer {
	private readonly source: string;
	private readonly headingDepthOffset: number;
	private readonly headingSlugPrefix: string;
	private readonly headings: TocHeading[] = [];
	private readonly slugCounts = new Map<string, number>();
	private index = 0;

	constructor(source: string, headingDepthOffset: number, headingSlugPrefix: string) {
		this.source = source;
		this.headingDepthOffset = headingDepthOffset;
		this.headingSlugPrefix = headingSlugPrefix;
	}

	render() {
		return {
			html: this.renderBlocks([]).trim(),
			headings: this.headings,
		};
	}

	private renderBlocks(stopEnvironments: string[], stopOnItem = false): string {
		const parts: string[] = [];

		while (this.index < this.source.length) {
			this.skipWhitespace();

			if (this.peekEndEnvironment(stopEnvironments)) {
				this.consumeEnvironmentEnd();
				break;
			}

			if (stopOnItem && this.peekCommand('item')) {
				break;
			}

			if (this.peekSection('section')) {
				parts.push(this.renderSection('section', 1));
				continue;
			}

			if (this.peekSection('subsection')) {
				parts.push(this.renderSection('subsection', 2));
				continue;
			}

			if (this.peekSection('subsubsection')) {
				parts.push(this.renderSection('subsubsection', 2));
				continue;
			}

			if (this.peekCommand('paragraph')) {
				parts.push(this.renderParagraphHeading());
				continue;
			}

			if (this.peekCommand('begin')) {
				parts.push(this.renderEnvironment());
				continue;
			}

			const text = this.readTextUntilBoundary(stopEnvironments, stopOnItem);
			if (text.trim()) {
				parts.push(this.renderParagraphs(text));
				continue;
			}

			this.index += 1;
		}

		return parts.join('\n');
	}

	private renderParagraphs(value: string) {
		const paragraphs = value
			.split(/\n\s*\n+/)
			.map((paragraph) => paragraph.trim())
			.filter(Boolean);

		return paragraphs
			.map((paragraph) => {
				const normalized = paragraph.replace(/[ \t]*\n[ \t]*/g, ' ');
				if (/^(\$\$[\s\S]*\$\$|\\\[[\s\S]*\\\])$/.test(normalized.trim())) {
					return `<div class="cp-content__math">${escapeHtml(normalized.trim())}</div>`;
				}

				return `<p>${renderInlineText(normalized)}</p>`;
			})
			.join('\n');
	}

	private renderSection(commandName: 'section' | 'subsection' | 'subsubsection', relativeDepth: number) {
		this.consumeCommand(commandName);
		if (this.source[this.index] === '*') {
			this.index += 1;
		}
		this.skipWhitespace();
		const titleGroup = readBalancedGroup(this.source, this.index, '{', '}');
		if (!titleGroup) {
			return '';
		}

		this.index = titleGroup.nextIndex;
		const plainTitle = stripLatexForPlainText(titleGroup.content) || `${capitalize(commandName)} heading`;
		const slugBase = normalizeSlug(plainTitle) || commandName;
		const slugCount = this.slugCounts.get(slugBase) ?? 0;
		this.slugCounts.set(slugBase, slugCount + 1);
		const uniqueSlug = slugCount === 0 ? slugBase : `${slugBase}-${slugCount + 1}`;
		const slug = this.headingSlugPrefix ? `${this.headingSlugPrefix}-${uniqueSlug}` : uniqueSlug;
		const depth = this.headingDepthOffset + relativeDepth;
		const level = Math.min(depth, 4);

		this.headings.push({
			depth,
			slug,
			text: plainTitle,
		});

		return `<h${level} id="${slug}">${renderInlineText(titleGroup.content.trim())}</h${level}>`;
	}

	private renderParagraphHeading() {
		this.consumeCommand('paragraph');
		this.skipWhitespace();
		const headingGroup = readBalancedGroup(this.source, this.index, '{', '}');
		if (!headingGroup) {
			return '';
		}

		this.index = headingGroup.nextIndex;
		return `<p class="cp-content__paragraph-heading"><strong>${renderInlineText(headingGroup.content.trim())}</strong></p>`;
	}

	private renderEnvironment() {
		const environment = this.consumeEnvironmentStart();
		if (!environment) {
			return '';
		}

		switch (environment.name) {
			case 'itemize':
				return this.renderList('ul', 'itemize');
			case 'enumerate':
				return this.renderList('ol', 'enumerate');
			case 'center':
				return `<div class="cp-content__center">${this.renderBlocks([environment.name])}</div>`;
			case 'quote':
				return `<blockquote>${this.renderBlocks([environment.name])}</blockquote>`;
			case 'lstlisting':
			case 'verbatim':
				return this.renderCodeEnvironment(environment.name);
			case 'tabular':
				return this.renderTabular();
			default:
				if (BLOCK_MATH_ENVIRONMENTS.has(environment.name)) {
					return this.renderMathEnvironment(environment.name);
				}
				if (THEOREM_LIKE_ENVIRONMENTS.has(environment.name)) {
					return this.renderTheoremLike(environment.name, environment.optionalArgument);
				}
				return this.renderUnknownEnvironment(environment.name);
		}
	}

	private renderList(tagName: 'ul' | 'ol', environmentName: 'itemize' | 'enumerate') {
		const items: string[] = [];

		while (this.index < this.source.length) {
			this.skipWhitespace();

			if (this.peekEndEnvironment([environmentName])) {
				this.consumeEnvironmentEnd();
				break;
			}

			if (!this.peekCommand('item')) {
				const stray = this.readTextUntilBoundary([environmentName], true).trim();
				if (stray) {
					items.push(`<li>${this.renderParagraphs(stray)}</li>`);
					continue;
				}
				break;
			}

			this.consumeCommand('item');
			this.skipWhitespace();

			let itemLabel = '';
			if (this.source[this.index] === '[') {
				const labelGroup = readBalancedGroup(this.source, this.index, '[', ']');
				if (labelGroup) {
					itemLabel = labelGroup.content.trim();
					this.index = labelGroup.nextIndex;
				}
			}

			const itemBody = this.renderBlocks([environmentName], true).trim();
			const labelMarkup = itemLabel
				? `<p class="cp-content__item-label"><strong>${renderInlineText(itemLabel)}</strong></p>`
				: '';
			items.push(`<li>${labelMarkup}${itemBody}</li>`);
		}

		return `<${tagName}>${items.join('')}</${tagName}>`;
	}

	private renderCodeEnvironment(environmentName: string) {
		const rawContent = this.readRawEnvironmentContent(environmentName);
		return `<pre class="cp-content__code"><code>${escapeHtml(rawContent.trimEnd())}</code></pre>`;
	}

	private renderMathEnvironment(environmentName: string) {
		const rawContent = this.readRawEnvironmentContent(environmentName);
		return `<div class="cp-content__math">${escapeHtml(`\\begin{${environmentName}}${rawContent}\\end{${environmentName}}`)}</div>`;
	}

	private renderTheoremLike(environmentName: string, optionalArgument?: string) {
		const label = capitalize(environmentName);
		const body = this.renderBlocks([environmentName]).trim();
		const suffix = optionalArgument ? ` (${renderInlineText(optionalArgument)})` : '';

		return [
			`<section class="cp-content__callout cp-content__callout--${environmentName}">`,
			`<p class="cp-content__callout-label"><strong>${label}${suffix}.</strong></p>`,
			body,
			'</section>',
		].join('');
	}

	private renderTabular() {
		const rawContent = this.readRawEnvironmentContent('tabular');
		const rows = rawContent
			.split(/\\\\\s*/g)
			.map((row) => row.replace(/\\(?:hline|cline\{[^}]*\})/g, '').trim())
			.filter(Boolean)
			.map((row) =>
				row
					.split(/(?<!\\)&/g)
					.map((cell) => cell.trim())
					.filter(Boolean),
			)
			.filter((row) => row.length > 0);

		if (!rows.length) {
			return `<pre class="cp-content__code"><code>${escapeHtml(rawContent.trim())}</code></pre>`;
		}

		const body = rows
			.map(
				(row) =>
					`<tr>${row.map((cell) => `<td>${renderInlineText(cell)}</td>`).join('')}</tr>`,
			)
			.join('');

		return `<div class="cp-content__table"><table><tbody>${body}</tbody></table></div>`;
	}

	private renderUnknownEnvironment(environmentName: string) {
		const rawContent = this.readRawEnvironmentContent(environmentName);
		return `<pre class="cp-content__code"><code>${escapeHtml(
			`\\begin{${environmentName}}${rawContent}\\end{${environmentName}}`,
		)}</code></pre>`;
	}

	private readRawEnvironmentContent(environmentName: string) {
		const endToken = `\\end{${environmentName}}`;
		const endIndex = this.source.indexOf(endToken, this.index);

		if (endIndex === -1) {
			const remaining = this.source.slice(this.index);
			this.index = this.source.length;
			return remaining;
		}

		const content = this.source.slice(this.index, endIndex);
		this.index = endIndex + endToken.length;
		return content;
	}

	private readTextUntilBoundary(stopEnvironments: string[], stopOnItem: boolean) {
		const start = this.index;

		while (this.index < this.source.length) {
			if (this.peekEndEnvironment(stopEnvironments)) {
				break;
			}
			if (stopOnItem && this.peekCommand('item')) {
				break;
			}
			if (this.peekSection('section') || this.peekSection('subsection') || this.peekSection('subsubsection')) {
				break;
			}
			if (this.peekCommand('paragraph') || this.peekCommand('begin')) {
				break;
			}
			this.index += 1;
		}

		return this.source.slice(start, this.index);
	}

	private skipWhitespace() {
		while (this.index < this.source.length && /\s/.test(this.source[this.index] ?? '')) {
			this.index += 1;
		}
	}

	private peekSection(name: 'section' | 'subsection' | 'subsubsection') {
		return this.source.startsWith(`\\${name}`, this.index);
	}

	private peekCommand(name: string) {
		if (!this.source.startsWith(`\\${name}`, this.index)) {
			return false;
		}
		const nextCharacter = this.source[this.index + name.length + 1];
		return !nextCharacter || !/[A-Za-z@]/.test(nextCharacter);
	}

	private consumeCommand(name: string) {
		if (this.peekCommand(name)) {
			this.index += name.length + 1;
		}
	}

	private peekEndEnvironment(environmentNames: string[]) {
		return environmentNames.some((environmentName) =>
			this.source.startsWith(`\\end{${environmentName}}`, this.index),
		);
	}

	private consumeEnvironmentEnd() {
		if (!this.source.startsWith('\\end{', this.index)) {
			return;
		}

		const environmentGroup = readBalancedGroup(this.source, this.index + 4, '{', '}');
		if (!environmentGroup) {
			return;
		}

		this.index = environmentGroup.nextIndex;
	}

	private consumeEnvironmentStart() {
		if (!this.peekCommand('begin')) {
			return null;
		}

		const environmentGroup = readBalancedGroup(this.source, this.index + 6, '{', '}');
		if (!environmentGroup) {
			return null;
		}

		this.index = environmentGroup.nextIndex;
		this.skipWhitespace();

		let optionalArgument: string | undefined;
		if (this.source[this.index] === '[') {
			const optionGroup = readBalancedGroup(this.source, this.index, '[', ']');
			if (optionGroup) {
				optionalArgument = optionGroup.content.trim();
				this.index = optionGroup.nextIndex;
				this.skipWhitespace();
			}
		}

		if (environmentGroup.content === 'tabular' && this.source[this.index] === '{') {
			const alignmentGroup = readBalancedGroup(this.source, this.index, '{', '}');
			if (alignmentGroup) {
				this.index = alignmentGroup.nextIndex;
				this.skipWhitespace();
			}
		}

		return {
			name: environmentGroup.content,
			optionalArgument,
		};
	}
}

export function renderLatexContent(
	texSource: string,
	options: { headingDepthOffset?: number; headingSlugPrefix?: string } = {},
): RenderedLatexContent {
	const source = texSource.trim();
	if (!source) {
		return {
			html: '',
			headings: [],
			usedFallback: false,
		};
	}

	try {
		const renderer = new LatexHtmlRenderer(
			source,
			options.headingDepthOffset ?? 2,
			options.headingSlugPrefix ?? 'editorial',
		);
		const result = renderer.render();

		if (!result.html) {
			return {
				html: `<pre class="cp-content__code"><code>${escapeHtml(source)}</code></pre>`,
				headings: [],
				usedFallback: true,
			};
		}

		return {
			html: result.html,
			headings: result.headings,
			usedFallback: false,
		};
	} catch {
		return {
			html: `<pre class="cp-content__code"><code>${escapeHtml(source)}</code></pre>`,
			headings: [],
			usedFallback: true,
		};
	}
}

export function renderStatementText(value: string) {
	return renderPlainTextStatement(value);
}
