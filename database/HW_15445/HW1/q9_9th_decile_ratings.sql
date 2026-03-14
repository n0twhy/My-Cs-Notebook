WITH PersonAvgRatings AS (
    SELECT 
        p.person_id,
        p.name,
        round(avg(r.rating), 2) AS avg_rating
    FROM people AS p
    JOIN crew AS c ON c.person_id = p.person_id
    JOIN titles AS t ON c.title_id = t.title_id
    JOIN ratings AS r ON r.title_id = t.title_id
    WHERE p.born = 1955
        AND t."type" = 'movie'
    GROUP BY p.person_id, p.name
),
PersonDeciles AS (
    SELECT 
        name,
        avg_rating,
        ntile(10) OVER (ORDER BY avg_rating ASC) AS decile
    FROM PersonAvgRatings
)
SELECT name, avg_rating
FROM PersonDeciles
WHERE decile = 9
ORDER BY avg_rating DESC, name ASC;