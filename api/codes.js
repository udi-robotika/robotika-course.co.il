import {list,put} from '@vercel/blob';

const PREFIX = 'robotika-course-codes/';
const MAX_TITLE = 80;
const MAX_CODE = 50000;
const recentPosts = new Map();

function reply(res,status,data) {
  res.setHeader('Content-Type','application/json; charset=utf-8');
  res.setHeader('Cache-Control','no-store');
  return res.status(status).json(data);
}

function storageReady() {
  return Boolean(process.env.BLOB_READ_WRITE_TOKEN || process.env.VERCEL_OIDC_TOKEN);
}

async function readBody(req) {
  if (req.body && typeof req.body === 'object') return req.body;
  if (typeof req.body === 'string') return JSON.parse(req.body);
  let raw = '';
  for await (const chunk of req) {
    raw += chunk;
    if (raw.length > MAX_CODE + 2000) throw new Error('too-large');
  }
  return JSON.parse(raw || '{}');
}

async function getCodes(res) {
  const blobs = [];
  let cursor;
  let hasMore = true;

  while (hasMore && blobs.length < 1000) {
    const result = await list(cursor ? {prefix:PREFIX,cursor} : {prefix:PREFIX});
    blobs.push(...result.blobs.filter(blob => blob.pathname.endsWith('.json')));
    cursor = result.cursor;
    hasMore = Boolean(result.hasMore && cursor);
  }

  const items = [];
  for (let start = 0; start < blobs.length; start += 20) {
    const batch = blobs.slice(start,start+20);
    const loaded = await Promise.all(batch.map(async blob => {
      for (let attempt = 0; attempt < 2; attempt++) {
        try {
          const response = await fetch(blob.downloadUrl || blob.url,{cache:'no-store'});
          if (response.ok) return await response.json();
        } catch {}
      }
      return null;
    }));
    items.push(...loaded.filter(Boolean));
  }

  items.sort((a,b) => Number(b.created)-Number(a.created));
  return reply(res,200,{items,count:items.length,storedFiles:blobs.length});
}

async function addCode(req,res) {
  const ip = String(req.headers['x-forwarded-for'] || req.socket?.remoteAddress || 'unknown').split(',')[0].trim();
  const now = Date.now();
  if (now-(recentPosts.get(ip)||0) < 1500) return reply(res,429,{error:'slow-down'});
  recentPosts.set(ip,now);

  const body = await readBody(req);
  if (body.website) return reply(res,200,{ok:true});
  const title = String(body.title || '').trim().slice(0,MAX_TITLE);
  const code = String(body.code || '').trim();
  if (!title || !code || code.length > MAX_CODE) return reply(res,400,{error:'invalid-code'});

  const item = {id:crypto.randomUUID(),title,code,created:now};
  const reverseTime = String(9999999999999-now).padStart(13,'0');
  await put(`${PREFIX}${reverseTime}-${item.id}.json`,JSON.stringify(item),{
    access:'public',
    addRandomSuffix:false,
    contentType:'application/json; charset=utf-8'
  });
  return reply(res,201,{ok:true,item});
}

export default async function handler(req,res) {
  if (!storageReady()) return reply(res,503,{error:'storage-not-configured'});
  try {
    if (req.method === 'GET') return await getCodes(res);
    if (req.method === 'POST') return await addCode(req,res);
    res.setHeader('Allow','GET, POST');
    return reply(res,405,{error:'method-not-allowed'});
  } catch (error) {
    console.error(error);
    return reply(res,500,{error:'server-error'});
  }
}
