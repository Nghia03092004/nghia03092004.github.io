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

Mac dinh site da duoc set san cho GitHub Pages root site:

```bash
npm run build
```

Neu can custom domain rieng, override `SITE_URL`:

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

De chuyen site nay thanh root site cua ban, doi ten repo thanh `nghia03092004.github.io`.

Sau khi rename, workflow se tu build voi:

- `SITE_URL=https://nghia03092004.github.io`
- `BASE_PATH=/`

Neu can domain rieng, sau nay co the them `public/CNAME`.

## Sync Project Euler statements

Trang problem Euler co the render full statement tu local TeX archive trong `../Project Euler/build_statement_books`, kem theo image/gif neu problem co media.

De dong bo lai toan bo statement:

```bash
npm run sync:project-euler-statements -- --all
```

Co the dong bo mot problem hoac mot khoang:

```bash
npm run sync:project-euler-statements -- --problem 96
npm run sync:project-euler-statements -- --from 1 --to 100
```

Script se:

- build HTML tu local TeX statement books bang `make4ht`
- tach moi problem thanh mot fragment rieng
- copy media tu `../Project Euler/Images/` ve `public/project-euler-media/`
- luu fragment vao `project_euler_unified/problem_XXX/statement.html`

Neu local TeX archive khong co compile-safe fragment cho mot problem, script se giu lai `statement.html` hien co lam fallback. Site se uu tien render statement nay o dau trang problem, va tu dong bo phan `## Problem Statement` trong file `solution.md` de tranh lap noi dung.

## Ghi chu ve Project Euler

Main problem content cua Project Euler duoc cap phep theo `CC BY-NC-SA 4.0`. Neu ban public statement mirror tren site, can giu attribution va khong dung cho muc dich thuong mai. Xem them tai `https://projecteuler.net/copyright`.
