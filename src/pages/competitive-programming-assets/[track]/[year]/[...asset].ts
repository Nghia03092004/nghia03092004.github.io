import { promises as fs } from 'node:fs';

import type { APIRoute } from 'astro';

import { getCompetitiveProgrammingAssetMap } from '../../../../features/competitive-programming/lib/competitive-programming';

interface AssetProps {
	absolutePath: string;
	contentType: string;
}

export async function getStaticPaths() {
	const assets = await getCompetitiveProgrammingAssetMap();

	return [...assets.entries()].map(([key, value]) => {
		const [track, year, ...assetParts] = key.split('/');

		return {
			params: {
				track,
				year,
				asset: assetParts.join('/'),
			},
			props: value,
		};
	});
}

export const GET: APIRoute<AssetProps> = async ({ props }) => {
	const body = await fs.readFile(props.absolutePath);

	return new Response(body, {
		headers: {
			'Content-Type': props.contentType,
		},
	});
};
