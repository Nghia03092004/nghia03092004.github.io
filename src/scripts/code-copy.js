const COPY_LABEL = 'Copy';
const COPIED_LABEL = 'Copied!';
const ERROR_LABEL = 'Unavailable';
const RESET_DELAY_MS = 1500;
const resetTimers = new WeakMap();

let generatedTargetCount = 0;

function setButtonLabel(button, label) {
	button.textContent = label;
}

function resetButtonLabel(button) {
	const defaultLabel = button.dataset.copyDefaultLabel || COPY_LABEL;
	setButtonLabel(button, defaultLabel);
}

function scheduleReset(button) {
	const previousTimer = resetTimers.get(button);
	if (previousTimer) {
		window.clearTimeout(previousTimer);
	}

	const nextTimer = window.setTimeout(() => {
		resetButtonLabel(button);
		resetTimers.delete(button);
	}, RESET_DELAY_MS);

	resetTimers.set(button, nextTimer);
}

async function fallbackCopyText(text) {
	const textArea = document.createElement('textarea');
	textArea.value = text;
	textArea.setAttribute('readonly', '');
	textArea.style.position = 'fixed';
	textArea.style.opacity = '0';
	textArea.style.pointerEvents = 'none';
	document.body.append(textArea);
	textArea.select();

	try {
		if (!document.execCommand('copy')) {
			throw new Error('Copy command was rejected.');
		}
	} finally {
		textArea.remove();
	}
}

async function writeClipboardText(text) {
	if (navigator.clipboard?.writeText) {
		await navigator.clipboard.writeText(text);
		return;
	}

	await fallbackCopyText(text);
}

function ensureCopyTargetId(codeElement) {
	if (codeElement.id) {
		return codeElement.id;
	}

	generatedTargetCount += 1;
	const generatedId = `code-copy-target-${generatedTargetCount}`;
	codeElement.id = generatedId;
	return generatedId;
}

function bindCopyButton(button) {
	if (!(button instanceof HTMLButtonElement) || button.dataset.copyBound === 'true') {
		return;
	}

	const defaultLabel = button.textContent?.trim() || COPY_LABEL;
	button.dataset.copyDefaultLabel = defaultLabel;

	button.addEventListener('click', async () => {
		const targetId = button.dataset.copyTarget;
		const target = targetId ? document.getElementById(targetId) : null;
		const text = target?.textContent ?? '';

		if (!text) {
			return;
		}

		try {
			await writeClipboardText(text);
			setButtonLabel(button, COPIED_LABEL);
		} catch {
			setButtonLabel(button, ERROR_LABEL);
		}

		scheduleReset(button);
	});

	button.dataset.copyBound = 'true';
}

function injectGenericCopyButtons() {
	document.querySelectorAll('pre > code').forEach((codeElement) => {
		if (!(codeElement instanceof HTMLElement)) {
			return;
		}

		const pre = codeElement.parentElement;
		if (!(pre instanceof HTMLPreElement) || pre.dataset.copyEnhanced === 'true') {
			return;
		}

		if (pre.closest('[data-source-panel]') || pre.closest('.cp-statement__sample-panel')) {
			return;
		}

		const wrapper = document.createElement('div');
		wrapper.className = 'code-copy-shell';
		pre.insertAdjacentElement('beforebegin', wrapper);
		wrapper.append(pre);

		const button = document.createElement('button');
		button.type = 'button';
		button.className = 'source-copy code-copy-button';
		button.dataset.copyTarget = ensureCopyTargetId(codeElement);
		button.textContent = COPY_LABEL;
		wrapper.append(button);

		pre.dataset.copyEnhanced = 'true';
		bindCopyButton(button);
	});
}

function setupCodeCopy() {
	injectGenericCopyButtons();

	document.querySelectorAll('[data-copy-target]').forEach((button) => {
		bindCopyButton(button);
	});
}

if (document.readyState === 'loading') {
	document.addEventListener('DOMContentLoaded', setupCodeCopy, { once: true });
} else {
	setupCodeCopy();
}

document.addEventListener('astro:page-load', setupCodeCopy);
