import { readFile } from 'node:fs/promises';

import type { APIRoute } from 'astro';

import { getDsaAssetMap } from '../../../features/competitive-programming-dsa/lib/dsa';

export async function getStaticPaths() {
	const assetMap = await getDsaAssetMap();

	return [...assetMap.keys()].map((asset) => ({
		params: {
			asset,
		},
	}));
}

export const GET: APIRoute = async ({ params }) => {
	const assetParam = params.asset;
	const assetKey = Array.isArray(assetParam) ? assetParam.join('/') : assetParam;
	if (!assetKey) {
		return new Response('Missing DSA asset path.', { status: 400 });
	}

	const assetMap = await getDsaAssetMap();
	const asset = assetMap.get(assetKey);
	if (!asset) {
		return new Response('DSA asset not found.', { status: 404 });
	}

	return new Response(await readFile(asset.absolutePath), {
		headers: {
			'Content-Type': asset.contentType,
		},
	});
};
