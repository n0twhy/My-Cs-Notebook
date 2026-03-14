SELECT name, 2026 - born AS age
FROM people
WHERE died IS NULL
    AND born >= 1900
ORDER BY born, name ASC
LIMIT 20;