SELECT NAME, count(person_id) AS NUM_APPEARANCES 
FROM crew AS c
JOIN people AS p
    ON c.person_id = p.person_id
GROUP BY person_id
ORDER BY NUM_APPEARANCES DESC
LIMIT 20;

SELECT p.name, count(c.person_id) AS NUM_APPEARANCES 
FROM crew AS c
JOIN people AS p
    ON c.person_id = p.person_id
GROUP BY c.person_id
ORDER BY NUM_APPEARANCES  DESC
limit 20;


