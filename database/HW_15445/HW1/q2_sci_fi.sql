SELECT *
FROM titles
LIMIT 10;

SELECT primary_title, genres, premiered, runtime_minutes || '(mins)' as runtime_minutes
FROM titles
WHERE genres LIKE '%Sci-Fi%'
    AND runtime_minutes IS NOT NULL
ORDER BY titles.runtime_minutes DESC  --*前面的||'(mins)'就是字符串，他会99>200，要按照原来的int类型排列，这里需要order by titles.runtime_minutes
LIMIT 10;