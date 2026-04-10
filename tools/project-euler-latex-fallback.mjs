import { promises as fs } from 'node:fs';
import path from 'node:path';

const MATH_ENVIRONMENTS = new Set([
	'align',
	'align*',
	'alignat*',
	'array',
	'cases',
	'equation',
	'equation*',
	'gather*',
	'matrix',
	'pmatrix',
	'split',
]);

const BLOCK_ENVIRONMENTS = new Set([
	'center',
	'enumerate',
	'flushleft',
	'footnotesize',
	'itemize',
	'lstlisting',
	'minipage',
	'multicols',
	'mybox_col',
	'small',
	'tabular',
]);

const LATEX_COLOR_MAP = {
	grey: 'gray',
	oranlge: 'orange',
};

export async function extractBookLatexStatements({ bookBuildRoot, placeholderMarker }, book, requestedProblems) {
	const requested = new Set(requestedProblems);
	const sourcePath = path.join(bookBuildRoot, 'generated', `main ${book}.tex`);
	const source = await fs.readFile(sourcePath, 'utf8');
	const sections = parseBookSections(source);
	const results = [];

	for (const section of sections) {
		if (!requested.has(section.problem)) {
			continue;
		}

		const placeholder =
			section.title.trim() === 'Archive status' || section.body.includes(placeholderMarker);
		const rendered = placeholder ? { html: '', assets: [], links: [] } : convertLatexToHtml(section.body);

		results.push({
			problem: section.problem,
			html: rendered.html,
			assets: rendered.assets,
			links: rendered.links,
			placeholder,
			sourceType: 'latex',
		});
	}

	return results;
}

function parseBookSections(source) {
	const sections = [];
	const matches = [...source.matchAll(/\\section\{Problem\s+(\d+):/g)];

	for (let index = 0; index < matches.length; index += 1) {
		const match = matches[index];
		const nextMatch = matches[index + 1];
		const sectionSource = source.slice(match.index, nextMatch?.index ?? source.length);
		const mybox = extractMybox(sectionSource);
		if (!mybox) {
			continue;
		}

		sections.push({
			problem: Number(match[1]),
			title: mybox.title,
			body: mybox.body,
		});
	}

	return sections;
}

function extractMybox(sectionSource) {
	const beginToken = '\\begin{mybox}';
	const endToken = '\\end{mybox}';
	const beginIndex = sectionSource.indexOf(beginToken);
	if (beginIndex === -1) {
		return null;
	}

	let cursor = skipWhitespace(sectionSource, beginIndex + beginToken.length);
	if (sectionSource[cursor] === '[') {
		cursor = readBalanced(sectionSource, cursor, '[', ']').endIndex;
	}

	cursor = skipWhitespace(sectionSource, cursor);
	if (sectionSource[cursor] !== '{') {
		return null;
	}

	const title = readBalanced(sectionSource, cursor, '{', '}');
	const bodyStart = title.endIndex;
	const bodyEnd = sectionSource.indexOf(endToken, bodyStart);
	if (bodyEnd === -1) {
		return null;
	}

	return {
		title: title.value,
		body: sectionSource.slice(bodyStart, bodyEnd),
	};
}

function convertLatexToHtml(source) {
	const context = {
		assets: new Set(),
		links: new Set(),
	};

	const normalized = normalizeLatexSource(source);
	const html = renderBlocks(normalized, context).trim();
	return {
		html,
		assets: [...context.assets],
		links: [...context.links],
	};
}

function normalizeLatexSource(source) {
	return collapseBlankLinesInMathBlocks(
		stripLatexComments(source)
		.replace(/\r\n?/g, '\n')
		.replace(/\t/g, ' ')
		.replace(/={3,}/g, '')
		.replace(/\\\[\s*\\begin\{tabular\}/g, '\\begin{tabular}')
		.replace(/\\end\{tabular\}\s*\\\]/g, '\\end{tabular}')
		.replace(/\$\$\s*\\begin\{tabular\}/g, '\\begin{tabular}')
		.replace(/\\end\{tabular\}\s*\$\$/g, '\\end{tabular}'),
	);
}

function stripLatexComments(source) {
	return source
		.split('\n')
		.map((line) => {
			let escaped = false;
			for (let index = 0; index < line.length; index += 1) {
				const character = line[index];
				if (character === '%' && !escaped) {
					return line.slice(0, index);
				}

				escaped = character === '\\' ? !escaped : false;
			}

			return line;
		})
		.join('\n');
}

function collapseBlankLinesInMathBlocks(source) {
	let result = source
		.replace(/\\\[[\s\S]*?\\\]/g, (match) => match.replace(/\n\s*\n+/g, '\n'))
		.replace(/\$\$[\s\S]*?\$\$/g, (match) => match.replace(/\n\s*\n+/g, '\n'));

	for (const environment of MATH_ENVIRONMENTS) {
		const pattern = new RegExp(
			String.raw`\\begin\{${escapeRegExp(environment)}\}[\s\S]*?\\end\{${escapeRegExp(environment)}\}`,
			'g',
		);
		result = result.replace(pattern, (match) => match.replace(/\n\s*\n+/g, '\n'));
	}

	return result;
}

function renderBlocks(source, context) {
	let cursor = 0;
	let buffer = '';
	let html = '';

	while (cursor < source.length) {
		const token = findNextBlockToken(source, cursor);
		if (!token) {
			buffer += source.slice(cursor);
			break;
		}

		buffer += source.slice(cursor, token.index);
		html += renderParagraphBuffer(buffer, context);
		buffer = '';

		if (token.type === 'command') {
			const command = readCommand(source, token.index);
			const rendered = renderBlockCommand(command.name, source, token.index, context);
			html += rendered.html;
			cursor = rendered.endIndex;
			continue;
		}

		const rendered = renderEnvironmentAt(source, token.name, token.index, context);
		html += rendered.html;
		cursor = rendered.endIndex;
	}

	html += renderParagraphBuffer(buffer, context);
	return html;
}

function findNextBlockToken(source, fromIndex) {
	let earliest = null;

	for (const environment of BLOCK_ENVIRONMENTS) {
		const pattern = `\\begin{${environment}}`;
		const index = source.indexOf(pattern, fromIndex);
		if (index !== -1 && (!earliest || index < earliest.index)) {
			earliest = { type: 'environment', name: environment, index };
		}
	}

	for (const command of ['\\vspace', '\\par', '\\noindent', '\\hfill', '\\footnotesize']) {
		const index = source.indexOf(command, fromIndex);
		if (index === -1) {
			continue;
		}

		if ((command === '\\par' || command === '\\hfill') && /[A-Za-z]/.test(source[index + command.length] ?? '')) {
			continue;
		}

		if (!earliest || index < earliest.index) {
			earliest = { type: 'command', name: command.slice(1), index };
		}
	}

	return earliest;
}

function renderBlockCommand(name, source, startIndex, context) {
	if (name === 'vspace') {
		const cursor = skipWhitespace(source, readCommand(source, startIndex).endIndex);
		if (source[cursor] !== '{') {
			return { html: '', endIndex: cursor };
		}

		const argument = readBalanced(source, cursor, '{', '}');
		const size = argument.value.includes('\\baselineskip') ? 'baseline' : 'small';
		return {
			html: `<div class="statement-spacer statement-spacer--${size}"></div>`,
			endIndex: argument.endIndex,
		};
	}

	if (name === 'footnotesize') {
		const command = readCommand(source, startIndex);
		const cursor = skipWhitespace(source, command.endIndex);
		if (source[cursor] !== '{') {
			return { html: '', endIndex: command.endIndex };
		}

		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: `<div class="statement-${name}">${renderBlocks(argument.value, context)}</div>`,
			endIndex: argument.endIndex,
		};
	}

	if (name === 'hfill') {
		return {
			html: '<div class="statement-fill"></div>',
			endIndex: readCommand(source, startIndex).endIndex,
		};
	}

	return {
		html: '',
		endIndex: readCommand(source, startIndex).endIndex,
	};
}

function renderEnvironmentAt(source, environment, startIndex, context) {
	const beginToken = `\\begin{${environment}}`;
	let cursor = skipWhitespace(source, startIndex + beginToken.length);
	let optionalArgument = null;
	let requiredArgument = null;

	if (source[cursor] === '[') {
		const optional = readBalanced(source, cursor, '[', ']');
		optionalArgument = optional.value;
		cursor = skipWhitespace(source, optional.endIndex);
	}

	if (['minipage', 'multicols', 'tabular'].includes(environment) && source[cursor] === '{') {
		const required = readBalanced(source, cursor, '{', '}');
		requiredArgument = required.value;
		cursor = skipWhitespace(source, required.endIndex);
	}

	const bodyEnd = findEnvironmentEnd(source, environment, cursor);
	const body = source.slice(cursor, bodyEnd);
	const endIndex = bodyEnd + `\\end{${environment}}`.length;
	return {
		html: renderEnvironment(environment, { body, optionalArgument, requiredArgument }, context),
		endIndex,
	};
}

function renderEnvironment(environment, args, context) {
	if (environment === 'center' || environment === 'flushleft') {
		return `<div class="${environment}">${renderBlocks(args.body, context)}</div>`;
	}

	if (environment === 'itemize' || environment === 'enumerate') {
		return renderList(args.body, environment === 'enumerate', context);
	}

	if (environment === 'multicols') {
		const columnCount = Number.parseInt(args.requiredArgument ?? '2', 10);
		return `<div class="statement-multicols cols-${Number.isNaN(columnCount) ? 2 : columnCount}">${renderBlocks(args.body, context)}</div>`;
	}

	if (environment === 'tabular') {
		return renderTable(args.body, context);
	}

	if (environment === 'minipage') {
		const widthStyle = buildWidthStyle(args.requiredArgument);
		return `<div class="statement-minipage"${widthStyle ? ` style="${widthStyle}"` : ''}>${renderBlocks(args.body, context)}</div>`;
	}

	if (environment === 'small' || environment === 'footnotesize') {
		return `<div class="statement-${environment}">${renderBlocks(args.body, context)}</div>`;
	}

	if (environment === 'mybox_col') {
		return `<div class="statement-mybox-col">${renderBlocks(args.body, context)}</div>`;
	}

	if (environment === 'lstlisting') {
		return `<pre class="statement-code"><code>${escapeHtml(args.body.trim())}</code></pre>`;
	}

	return renderBlocks(args.body, context);
}

function renderList(source, ordered, context) {
	const items = splitListItems(source);

	if (!items.length) {
		return '';
	}

	const tag = ordered ? 'ol' : 'ul';
	return `<${tag}>${items.map((item) => `<li>${renderBlocks(item, context)}</li>`).join('')}</${tag}>`;
}

function splitListItems(source) {
	const items = [];
	let current = '';
	let braceDepth = 0;
	let inlineMath = false;
	let nestedListDepth = 0;
	let seenItem = false;

	for (let index = 0; index < source.length; index += 1) {
		const character = source[index];
		const nextCharacter = source[index + 1];

		if (character === '$' && !isEscaped(source, index) && nextCharacter !== '$') {
			inlineMath = !inlineMath;
			if (seenItem) {
				current += character;
			}
			continue;
		}

		if (!inlineMath) {
			if (character === '{') {
				braceDepth += 1;
			} else if (character === '}') {
				braceDepth = Math.max(0, braceDepth - 1);
			}
		}

		if (!inlineMath && braceDepth === 0) {
			const beginToken = readNestedListToken(source, index, 'begin');
			if (beginToken) {
				nestedListDepth += 1;
				if (seenItem) {
					current += beginToken.value;
				}
				index += beginToken.value.length - 1;
				continue;
			}

			const endToken = readNestedListToken(source, index, 'end');
			if (endToken) {
				nestedListDepth = Math.max(0, nestedListDepth - 1);
				if (seenItem) {
					current += endToken.value;
				}
				index += endToken.value.length - 1;
				continue;
			}

			if (
				nestedListDepth === 0 &&
				source.startsWith('\\item', index) &&
				!/[A-Za-z]/.test(source[index + '\\item'.length] ?? '')
			) {
				if (seenItem && current.trim()) {
					items.push(current.trim());
				}

				seenItem = true;
				current = '';
				index += '\\item'.length - 1;
				continue;
			}
		}

		if (seenItem) {
			current += character;
		}
	}

	if (seenItem && current.trim()) {
		items.push(current.trim());
	}

	return items;
}

function readNestedListToken(source, startIndex, kind) {
	for (const environment of ['itemize', 'enumerate']) {
		const token = `\\${kind}{${environment}}`;
		if (source.startsWith(token, startIndex)) {
			return { value: token };
		}
	}

	return null;
}

function renderTable(source, context) {
	const cleaned = source
		.replace(/\\hline/g, '')
		.replace(/\\hhline\{[^}]*\}/g, '')
		.replace(/\\cline\{[^}]*\}/g, '')
		.replace(/\\noalign\{[^}]*\}/g, '')
		.trim();

	const rows = splitTableRows(cleaned);

	if (!rows.length) {
		return '';
	}

	const body = rows
		.map((row) => {
			const cells = splitTableCells(row);
			return `<tr>${cells.map((cell) => renderTableCell(cell, context)).join('')}</tr>`;
		})
		.join('');

	return `<table class="statement-table"><tbody>${body}</tbody></table>`;
}

function splitTableRows(source) {
	const rows = [];
	let current = '';
	let braceDepth = 0;
	let inlineMath = false;

	for (let index = 0; index < source.length; index += 1) {
		const character = source[index];
		const nextCharacter = source[index + 1];

		if (character === '$' && !isEscaped(source, index) && nextCharacter !== '$') {
			inlineMath = !inlineMath;
			current += character;
			continue;
		}

		if (!inlineMath) {
			if (character === '{') {
				braceDepth += 1;
			} else if (character === '}') {
				braceDepth = Math.max(0, braceDepth - 1);
			}
		}

		if (
			character === '\\' &&
			nextCharacter === '\\' &&
			braceDepth === 0 &&
			!inlineMath &&
			!isEscaped(source, index)
		) {
			rows.push(current.trim());
			current = '';
			index += 1;

			if (source[index + 1] === '[') {
				const optional = readBalanced(source, index + 1, '[', ']');
				index = optional.endIndex - 1;
			}

			continue;
		}

		current += character;
	}

	if (current.trim()) {
		rows.push(current.trim());
	}

	return rows.filter(Boolean);
}

function splitTableCells(row) {
	const cells = [];
	let current = '';
	let braceDepth = 0;
	let inlineMath = false;

	for (let index = 0; index < row.length; index += 1) {
		const character = row[index];
		const nextCharacter = row[index + 1];

		if (character === '$' && !isEscaped(row, index) && nextCharacter !== '$') {
			inlineMath = !inlineMath;
			current += character;
			continue;
		}

		if (!inlineMath) {
			if (character === '{') {
				braceDepth += 1;
			} else if (character === '}') {
				braceDepth = Math.max(0, braceDepth - 1);
			}
		}

		if (character === '&' && braceDepth === 0 && !inlineMath) {
			cells.push(current.trim());
			current = '';
			continue;
		}

		current += character;
	}

	if (current.trim()) {
		cells.push(current.trim());
	}

	return cells;
}

function renderTableCell(cell, context) {
	if (cell.startsWith('\\multicolumn')) {
		const parsed = readMacroArguments(cell, '\\multicolumn', 3);
		if (parsed) {
			const colspan = Number.parseInt(parsed.arguments[0], 10);
			return `<td${Number.isNaN(colspan) ? '' : ` colspan="${colspan}"`}>${renderInlineText(parsed.arguments[2], context)}</td>`;
		}
	}

	if (cell.startsWith('\\multirow')) {
		const parsed = readMacroArguments(cell, '\\multirow', 3);
		if (parsed) {
			const rowspan = Number.parseInt(parsed.arguments[0], 10);
			return `<td${Number.isNaN(rowspan) ? '' : ` rowspan="${rowspan}"`}>${renderInlineText(parsed.arguments[2], context)}</td>`;
		}
	}

	return `<td>${renderInlineText(cell, context)}</td>`;
}

function renderParagraphBuffer(buffer, context) {
	const segments = buffer
		.split(/\n\s*\n+/)
		.map((segment) => segment.trim())
		.filter(Boolean);

	return segments
		.map((segment) => {
			const inline = renderInlineText(segment.replace(/\s*\n\s*/g, ' '), context).trim();
			if (!inline) {
				return '';
			}

			return `<p class="noindent">${inline}</p>`;
		})
		.join('');
}

function renderInlineText(source, context) {
	const { text, segments } = protectMathSegments(source);
	let cursor = 0;
	let html = '';

	while (cursor < text.length) {
		const token = readMathToken(text, cursor);
		if (token) {
			html += token;
			cursor += token.length;
			continue;
		}

		if (text[cursor] === '\\') {
			const rendered = renderInlineCommand(text, cursor, context);
			if (rendered) {
				html += rendered.html;
				cursor = rendered.endIndex;
				continue;
			}
		}

		const character = text[cursor];
		if (character === '{' || character === '}') {
			cursor += 1;
			continue;
		}

		html += character === '~' ? ' ' : escapeHtml(character);
		cursor += 1;
	}

	return restoreMathTokens(html, segments);
}

function protectMathSegments(source) {
	const segments = [];
	let cursor = 0;
	let output = '';

	while (cursor < source.length) {
		const math = readMathSegment(source, cursor);
		if (!math) {
			output += source[cursor];
			cursor += 1;
			continue;
		}

		const token = `@@MATH${segments.length}@@`;
		segments.push(math.value);
		output += token;
		cursor = math.endIndex;
	}

	return {
		text: output,
		segments,
	};
}

function readMathSegment(source, startIndex) {
	if (source.startsWith('$$', startIndex) && !isEscaped(source, startIndex)) {
		const endIndex = findClosingDoubleDollar(source, startIndex + 2);
		if (endIndex !== -1) {
			return {
				value: source.slice(startIndex, endIndex + 2),
				endIndex: endIndex + 2,
			};
		}
	}

	if (source.startsWith('\\[', startIndex)) {
		const endIndex = source.indexOf('\\]', startIndex + 2);
		if (endIndex !== -1) {
			return {
				value: source.slice(startIndex, endIndex + 2),
				endIndex: endIndex + 2,
			};
		}
	}

	if (source.startsWith('\\(', startIndex)) {
		const endIndex = source.indexOf('\\)', startIndex + 2);
		if (endIndex !== -1) {
			return {
				value: source.slice(startIndex, endIndex + 2),
				endIndex: endIndex + 2,
			};
		}
	}

	if (source[startIndex] === '$' && !isEscaped(source, startIndex) && source[startIndex + 1] !== '$') {
		const endIndex = findClosingInlineDollar(source, startIndex + 1);
		if (endIndex !== -1) {
			return {
				value: source.slice(startIndex, endIndex + 1),
				endIndex: endIndex + 1,
			};
		}
	}

	if (source.startsWith('\\begin{', startIndex)) {
		const environment = readEnvironmentName(source, startIndex);
		if (environment && MATH_ENVIRONMENTS.has(environment.name)) {
			const bodyStart = environment.endIndex;
			const bodyEnd = findEnvironmentEnd(source, environment.name, bodyStart);
			const endIndex = bodyEnd + `\\end{${environment.name}}`.length;
			return {
				value: source.slice(startIndex, endIndex),
				endIndex,
			};
		}
	}

	return null;
}

function renderInlineCommand(source, startIndex, context) {
	const command = readCommand(source, startIndex);
	let cursor = skipWhitespace(source, command.endIndex);

	if (command.name === '\\') {
		return {
			html: '<br>',
			endIndex: command.endIndex,
		};
	}

	if (['%', '&', '$', '#', '_', '{', '}'].includes(command.name)) {
		return {
			html: escapeHtml(command.name),
			endIndex: command.endIndex,
		};
	}

	if (command.name === ',') {
		return {
			html: ' ',
			endIndex: command.endIndex,
		};
	}

	if (command.name === 'ldots' || command.name === 'cdots' || command.name === 'dots') {
		return {
			html: '&hellip;',
			endIndex: command.endIndex,
		};
	}

	if (command.name === 'newline') {
		return {
			html: '<br>',
			endIndex: command.endIndex,
		};
	}

	if (command.name === 'href') {
		const url = readBalanced(source, cursor, '{', '}');
		const label = readBalanced(source, skipWhitespace(source, url.endIndex), '{', '}');
		const href = url.value.trim();
		context.links.add(href);
		return {
			html: `<a href="${escapeAttribute(href)}">${renderInlineText(label.value, context)}</a>`,
			endIndex: label.endIndex,
		};
	}

	if (command.name === 'url') {
		const url = readBalanced(source, cursor, '{', '}');
		const href = url.value.trim();
		context.links.add(href);
		return {
			html: `<a href="${escapeAttribute(href)}">${escapeHtml(href)}</a>`,
			endIndex: url.endIndex,
		};
	}

	if (command.name === 'textit' || command.name === 'emph') {
		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: `<em>${renderInlineText(argument.value, context)}</em>`,
			endIndex: argument.endIndex,
		};
	}

	if (command.name === 'textbf') {
		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: `<strong>${renderInlineText(argument.value, context)}</strong>`,
			endIndex: argument.endIndex,
		};
	}

	if (command.name === 'underline') {
		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: `<span class="statement-underline">${renderInlineText(argument.value, context)}</span>`,
			endIndex: argument.endIndex,
		};
	}

	if (command.name === 'texttt') {
		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: `<code>${renderInlineText(argument.value, context)}</code>`,
			endIndex: argument.endIndex,
		};
	}

	if (command.name === 'textcolor') {
		const color = readBalanced(source, cursor, '{', '}');
		const value = readBalanced(source, skipWhitespace(source, color.endIndex), '{', '}');
		const cssColor = sanitizeColor(color.value);
		return {
			html: `<span${cssColor ? ` style="color:${cssColor}"` : ''}>${renderInlineText(value.value, context)}</span>`,
			endIndex: value.endIndex,
		};
	}

	if (command.name === 'tooltip') {
		const content = readBalanced(source, cursor, '{', '}');
		const label = readBalanced(source, skipWhitespace(source, content.endIndex), '{', '}');
		return {
			html: renderInlineText(content.value, context),
			endIndex: label.endIndex,
		};
	}

	if (command.name === 'myparagraph') {
		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: `<strong>${renderInlineText(argument.value, context)}</strong><br>`,
			endIndex: argument.endIndex,
		};
	}

	if (command.name === 'captionof') {
		const kind = readBalanced(source, cursor, '{', '}');
		const caption = readBalanced(source, skipWhitespace(source, kind.endIndex), '{', '}');
		return {
			html: `<span class="statement-caption">${renderInlineText(caption.value, context)}</span>`,
			endIndex: caption.endIndex,
		};
	}

	if (command.name === 'makecell') {
		const argument = readBalanced(source, cursor, '{', '}');
		const parts = argument.value
			.split(/\\\\/g)
			.map((part) => stripOuterBraces(part.trim()))
			.filter(Boolean)
			.map((part) => renderInlineText(part, context));
		return {
			html: parts.join('<br>'),
			endIndex: argument.endIndex,
		};
	}

	if (command.name === 'mbox') {
		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: renderInlineText(argument.value, context),
			endIndex: argument.endIndex,
		};
	}

	if (command.name === 'num') {
		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: escapeHtml(argument.value),
			endIndex: argument.endIndex,
		};
	}

	if (['footnotesize', 'large', 'normalsize', 'scriptsize', 'small', 'tiny'].includes(command.name) && source[cursor] === '{') {
		const argument = readBalanced(source, cursor, '{', '}');
		return {
			html: `<span class="statement-${command.name}">${renderInlineText(argument.value, context)}</span>`,
			endIndex: argument.endIndex,
		};
	}

	if (command.name === 'includegraphics') {
		let options = null;
		if (source[cursor] === '[') {
			options = readBalanced(source, cursor, '[', ']');
			cursor = skipWhitespace(source, options.endIndex);
		}

		const asset = readBalanced(source, cursor, '{', '}');
		const assetPath = asset.value.trim();
		context.assets.add(assetPath);
		return {
			html: renderImageTag(assetPath, options?.value ?? ''),
			endIndex: asset.endIndex,
		};
	}

	if (command.name === 'animategraphics') {
		let options = null;
		if (source[cursor] === '[') {
			options = readBalanced(source, cursor, '[', ']');
			cursor = skipWhitespace(source, options.endIndex);
		}

		const frameRate = readBalanced(source, cursor, '{', '}');
		const prefix = readBalanced(source, skipWhitespace(source, frameRate.endIndex), '{', '}');
		const start = readBalanced(source, skipWhitespace(source, prefix.endIndex), '{', '}');
		const end = readBalanced(source, skipWhitespace(source, start.endIndex), '{', '}');
		const assetPath = deriveAnimatedAssetPath(prefix.value.trim());
		context.assets.add(assetPath);
		return {
			html: renderImageTag(assetPath, options?.value ?? '', 'Problem animation'),
			endIndex: end.endIndex,
		};
	}

	if ([
		'cellcolor',
		'centering',
		'color',
		'hfill',
		'item',
		'medskip',
		'noindent',
		'par',
		'phantom',
		'rowcolor',
		'smallskip',
		'text',
	].includes(command.name)) {
		if (source[cursor] === '[') {
			cursor = readBalanced(source, cursor, '[', ']').endIndex;
			cursor = skipWhitespace(source, cursor);
		}

		if (source[cursor] === '{') {
			cursor = readBalanced(source, cursor, '{', '}').endIndex;
		}

		return {
			html: '',
			endIndex: cursor,
		};
	}

	if (source[cursor] === '{') {
		let argument;
		try {
			argument = readBalanced(source, cursor, '{', '}');
		} catch (error) {
			const preview = source.slice(Math.max(0, startIndex - 40), Math.min(source.length, startIndex + 120));
			throw new Error(`Failed to parse inline command \\${command.name}: ${preview}`);
		}
		return {
			html: renderInlineText(argument.value, context),
			endIndex: argument.endIndex,
		};
	}

	return {
		html: escapeHtml(`\\${command.name}`),
		endIndex: command.endIndex,
	};
}

function renderImageTag(assetPath, optionSource, alt = 'Problem illustration') {
	const options = parseOptionMap(optionSource);
	const style = [
		buildWidthStyle(options.width),
		buildLengthStyle('height', options.height),
	]
		.filter(Boolean)
		.join(';');

	return `<img src="${escapeAttribute(assetPath)}" alt="${escapeAttribute(alt)}"${style ? ` style="${style}"` : ''}>`;
}

function parseOptionMap(optionSource) {
	if (!optionSource) {
		return {};
	}

	return optionSource.split(',').reduce((result, entry) => {
		const [rawKey, rawValue] = entry.split('=').map((part) => part?.trim()).filter(Boolean);
		if (!rawKey) {
			return result;
		}

		if (rawValue) {
			result[rawKey] = rawValue;
		} else {
			result[rawKey] = true;
		}

		return result;
	}, {});
}

function buildWidthStyle(value) {
	return buildLengthStyle('width', value);
}

function buildLengthStyle(property, value) {
	if (!value) {
		return '';
	}

	const normalized = value.replace(/\s+/g, '');
	if (normalized === '\\linewidth' || normalized === '\\textwidth') {
		return `${property}:100%`;
	}

	const relativeMatch = normalized.match(/^([0-9]*\.?[0-9]+)\\(?:linewidth|textwidth)$/);
	if (relativeMatch) {
		return `${property}:${Number(relativeMatch[1]) * 100}%`;
	}

	const absoluteMatch = normalized.match(/^([0-9]*\.?[0-9]+)(cm|mm|in|pt|px)$/);
	if (absoluteMatch) {
		return `${property}:${absoluteMatch[1]}${absoluteMatch[2]}`;
	}

	return '';
}

function deriveAnimatedAssetPath(prefix) {
	const trimmed = prefix.replace(/[-_]+$/, '');
	return `${trimmed}.gif`;
}

function restoreMathTokens(source, segments) {
	return source.replace(/@@MATH(\d+)@@/g, (_, index) => segments[Number(index)] ?? '');
}

function readMathToken(source, startIndex) {
	const match = source.slice(startIndex).match(/^@@MATH\d+@@/);
	return match ? match[0] : null;
}

function findClosingDoubleDollar(source, fromIndex) {
	for (let index = fromIndex; index < source.length - 1; index += 1) {
		if (source.startsWith('$$', index) && !isEscaped(source, index)) {
			return index;
		}
	}

	return -1;
}

function findClosingInlineDollar(source, fromIndex) {
	for (let index = fromIndex; index < source.length; index += 1) {
		if (source[index] === '$' && !isEscaped(source, index) && source[index + 1] !== '$') {
			return index;
		}
	}

	return -1;
}

function readEnvironmentName(source, startIndex) {
	if (!source.startsWith('\\begin{', startIndex)) {
		return null;
	}

	const argument = readBalanced(source, startIndex + '\\begin'.length, '{', '}');
	return {
		name: argument.value,
		endIndex: argument.endIndex,
	};
}

function findEnvironmentEnd(source, environment, fromIndex) {
	const tokenPattern = new RegExp(String.raw`\\(begin|end)\{${escapeRegExp(environment)}\}`, 'g');
	tokenPattern.lastIndex = fromIndex;
	let depth = 1;
	let match;

	while ((match = tokenPattern.exec(source))) {
		depth += match[1] === 'begin' ? 1 : -1;
		if (depth === 0) {
			return match.index;
		}
	}

	throw new Error(`Unclosed LaTeX environment: ${environment}`);
}

function readMacroArguments(source, macro, count) {
	if (!source.startsWith(macro)) {
		return null;
	}

	const argumentsList = [];
	let cursor = skipWhitespace(source, macro.length);

	for (let index = 0; index < count; index += 1) {
		if (source[cursor] !== '{') {
			return null;
		}

		const argument = readBalanced(source, cursor, '{', '}');
		argumentsList.push(argument.value);
		cursor = skipWhitespace(source, argument.endIndex);
	}

	return {
		arguments: argumentsList,
		endIndex: cursor,
	};
}

function readCommand(source, startIndex) {
	let cursor = startIndex + 1;
	if (cursor >= source.length) {
		return { name: '', endIndex: cursor };
	}

	if (/[A-Za-z@]/.test(source[cursor])) {
		while (cursor < source.length && /[A-Za-z*@]/.test(source[cursor])) {
			cursor += 1;
		}

		return {
			name: source.slice(startIndex + 1, cursor),
			endIndex: cursor,
		};
	}

	return {
		name: source[cursor],
		endIndex: cursor + 1,
	};
}

function readBalanced(source, startIndex, openCharacter, closeCharacter) {
	if (source[startIndex] !== openCharacter) {
		throw new Error(`Expected ${openCharacter} at index ${startIndex}`);
	}

	let depth = 0;
	for (let index = startIndex; index < source.length; index += 1) {
		const character = source[index];
		if (character === openCharacter && !isEscaped(source, index)) {
			depth += 1;
		} else if (character === closeCharacter && !isEscaped(source, index)) {
			depth -= 1;
			if (depth === 0) {
				return {
					value: source.slice(startIndex + 1, index),
					endIndex: index + 1,
				};
			}
		}
	}

	throw new Error(`Unclosed ${openCharacter}${closeCharacter} block starting at index ${startIndex}`);
}

function sanitizeColor(value) {
	const trimmed = value.trim();
	const normalized = LATEX_COLOR_MAP[trimmed] ?? trimmed;
	return /^[A-Za-z]+$/.test(normalized) ? normalized : '';
}

function stripOuterBraces(value) {
	if (!value.startsWith('{') || !value.endsWith('}')) {
		return value;
	}

	try {
		const balanced = readBalanced(value, 0, '{', '}');
		return balanced.endIndex === value.length ? balanced.value : value;
	} catch {
		return value;
	}
}

function skipWhitespace(source, startIndex) {
	let cursor = startIndex;
	while (cursor < source.length && /\s/.test(source[cursor])) {
		cursor += 1;
	}

	return cursor;
}

function isEscaped(source, index) {
	let slashCount = 0;
	for (let cursor = index - 1; cursor >= 0 && source[cursor] === '\\'; cursor -= 1) {
		slashCount += 1;
	}

	return slashCount % 2 === 1;
}

function escapeHtml(value) {
	return value
		.replaceAll('&', '&amp;')
		.replaceAll('<', '&lt;')
		.replaceAll('>', '&gt;')
		.replaceAll('"', '&quot;');
}

function escapeAttribute(value) {
	return escapeHtml(value).replaceAll("'", '&#39;');
}

function escapeRegExp(value) {
	return value.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}
