SELECT s.id, s.name AS student_name, d.name AS department, s.gpa
FROM students AS s
INNER JOIN departments AS d ON s.department_id = d.id
WHERE s.gpa >= 3.50
ORDER BY s.gpa DESC
LIMIT 4;