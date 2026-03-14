SELECT ((t.premiered / 10) * 10) || 's' AS DECADE,
    round(avg(r.rating), 2) AS AVG_RATING,
    max(r.rating) AS TOP_RATING,
    min(r.rating) AS MIN_RATING,
    count(t.title_id) AS NUM_RELEASES
FROM titles AS t
JOIN ratings AS r
    ON t.title_id = r.title_id
WHERE t.primary_title IS NOT NULL
    AND t.premiered IS NOT NULL
    AND r.rating IS NOT NULL
GROUP BY DECADE
ORDER BY AVG_RATING DESC, DECADE ASC;
