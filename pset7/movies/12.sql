-- SELECT title FROM movies
-- JOIN stars on stars.movie_id = movies.id
-- JOIN people on people.id = stars.person_id
-- WHERE people.name = "Johnny Depp"
-- INTERSECT
-- SELECT title FROM movies
-- JOIN stars on stars.movie_id = movies.id
-- JOIN people on people.id = stars.person_id
-- WHERE people.name = "Helena Bonham Carter"


SELECT title FROM movies WHERE id IN
(SELECT movie_id FROM stars
JOIN people
ON stars.person_id = people.id
WHERE name = "Helena Bonham Carter" AND movie_id IN
(SELECT movie_id FROM stars WHERE person_id IN
(SELECT id FROM people WHERE name = "Johnny Depp")))