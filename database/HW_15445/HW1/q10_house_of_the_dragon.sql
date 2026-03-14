WITH RECURSIVE ordered_titles AS (
    SELECT DISTINCT
        a.title,
        row_number() OVER (ORDER BY a.title) AS rn
    FROM akas AS a
    WHERE a.title_id = (
        SELECT title_id
        FROM titles
        WHERE primary_title = 'House of the Dragon'
    )
),
Concat_CTE(rn, concatenated_string) AS (
    -- 基础行：第一个标题
    SELECT rn, title
    FROM ordered_titles
    WHERE rn = 1
    UNION ALL
    -- 递归行：往后依次拼接
    SELECT
        curr.rn,
        prev.concatenated_string || ', ' || curr.title
    FROM Concat_CTE AS prev
    JOIN ordered_titles AS curr
      ON curr.rn = prev.rn + 1
)
SELECT concatenated_string
FROM Concat_CTE
ORDER BY rn DESC
LIMIT 1;