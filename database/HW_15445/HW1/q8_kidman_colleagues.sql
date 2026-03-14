SELECT DISTINCT(p.name)
FROM crew as c
JOIN people as p ON p.person_id = c.person_id
WHERE c.title_id IN (
    SELECT c.title_id
    FROM crew as c
    JOIN people AS p ON p.person_id = c.person_id
    WHERE p.name = 'Nicole Kidman'
        AND p.born = 1967
)
    AND c.category IN ('actor', 'actress')
ORDER BY p.name;