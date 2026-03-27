# Notes & Numbers

Blog/site ca nhan duoc build bang Astro, chia ro 3 luong noi dung:

- `essays`: bai viet dai, ghi chep, suy nghi
- `code`: build logs, snippets, implementation notes
- `project-euler`: archive cho bai Project Euler, co filter theo status / difficulty / language

## Chay local

```bash
npm install
npm run dev
```

Build production:

```bash
npm run build
```

Neu deploy that su, set domain cho sitemap/RSS:

```bash
SITE_URL=https://your-domain.com npm run build
```

## Cho canh chinh nhanh

Sua metadata chung trong `src/site.config.ts`:

- ten site
- author
- email
- social links
- tagline

## Cau truc noi dung

Noi dung nam trong `src/data/`:

```text
src/data/
  essays/
  code/
  project-euler/
```

Chi can them file `.md` hoac `.mdx` vao dung thu muc la route se duoc tao tu dong.

## Mau frontmatter

Essay / code:

```yaml
---
title: Sample title
description: Short summary
published: 2026-03-26
updated: 2026-03-27 # optional
featured: false
draft: false
readingTime: 5 min
tags:
  - tag-one
  - tag-two
---
```

Project Euler:

```yaml
---
title: Problem title
description: Short summary
published: 2026-03-26
problem: 1
status: solved # solved | working | revisit
difficulty: warmup # warmup | medium | hard
languages:
  - Python
answer: "233168" # optional
tags:
  - arithmetic-series
---
```

## Routes chinh

- `/`: landing page
- `/essays`: danh sach essays
- `/code`: danh sach code notes
- `/project-euler`: archive Euler co filter
- `/about`: gioi thieu
- `/feed.xml`: RSS

## Deploy

Day la static site, co the deploy len:

- Vercel
- Netlify
- Cloudflare Pages
- GitHub Pages

## GitHub Pages

Repo da duoc them workflow tai `.github/workflows/deploy.yml`.

Chi can:

1. Tao repo tren GitHub.
2. `git remote add origin <repo-url>`
3. `git push -u origin master`

Workflow se tu build va deploy len GitHub Pages cho ca 2 truong hop:

- repo ten `username.github.io`
- repo thuong, deploy duoi duong dan `https://username.github.io/<repo>/`

Neu can domain rieng, sau nay co the them `public/CNAME`.

## Ghi chu ve Project Euler

Site nay da duoc thiet ke theo kieu archive/log, link ve bai goc tren Project Euler thay vi mirror nguyen de bai. Neu ban dang dinh dang full statement hoac solution chi tiet cho nhieu bai, nen tu kiem tra lai policy/licensing hien hanh cua Project Euler truoc khi public hang loat.
