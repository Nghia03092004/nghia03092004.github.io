const EXTERNAL_PROTOCOL = /^(?:[a-z]+:)?\/\//i;

export function withBase(path: string) {
	if (!path || EXTERNAL_PROTOCOL.test(path) || path.startsWith('mailto:') || path.startsWith('#')) {
		return path;
	}

	const normalizedBase = import.meta.env.BASE_URL.replace(/\/$/, '');

	if (path === '/') {
		return normalizedBase || '/';
	}

	return `${normalizedBase}${path}`;
}
