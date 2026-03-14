SELECT t.primary_title AS NAME,
    r.votes AS VOTES
FROM titles AS t 
JOIN crew AS c ON c.title_id = t.title_id
JOIN people AS p ON p.person_id = c.person_id
JOIN ratings AS r ON r.title_id = t.title_id
WHERE p.name LIKE '%Cruise%'
    AND p.born = 1962
ORDER BY VOTES DESC
LIMIT 10;

