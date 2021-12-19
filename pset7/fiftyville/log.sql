-- Keep a log of any SQL queries you execute as you solve the mystery.

-- All you know is that the theft took place on July 28, 2020 and that it took place on Chamberlin Street.

-- who is the thief, where they escaped to, and who are accomplices.



-- Get tables
.tables
-- airports                  crime_scene_reports       people
-- atm_transactions          flights                   phone_calls
-- bank_accounts             interviews
-- courthouse_security_logs  passengers

-- Get the schema
.schema
-- CREATE TABLE crime_scene_reports (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     street TEXT,
--     description TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE interviews (
--     id INTEGER,
--     name TEXT,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     transcript TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE courthouse_security_logs (
--     id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     activity TEXT,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE atm_transactions (
--     id INTEGER,
--     account_number INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     atm_location TEXT,
--     transaction_type TEXT,
--     amount INTEGER,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE bank_accounts (
--     account_number INTEGER,
--     person_id INTEGER,
--     creation_year INTEGER,
--     FOREIGN KEY(person_id) REFERENCES people(id)
-- );
-- CREATE TABLE airports (
--     id INTEGER,
--     abbreviation TEXT,
--     full_name TEXT,
--     city TEXT,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE flights (
--     id INTEGER,
--     origin_airport_id INTEGER,
--     destination_airport_id INTEGER,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     hour INTEGER,
--     minute INTEGER,
--     PRIMARY KEY(id),
--     FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
--     FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
-- );
-- CREATE TABLE passengers (
--     flight_id INTEGER,
--     passport_number INTEGER,
--     seat TEXT,
--     FOREIGN KEY(flight_id) REFERENCES flights(id)
-- );
-- CREATE TABLE phone_calls (
--     id INTEGER,
--     caller TEXT,
--     receiver TEXT,
--     year INTEGER,
--     month INTEGER,
--     day INTEGER,
--     duration INTEGER,
--     PRIMARY KEY(id)
-- );
-- CREATE TABLE people (
--     id INTEGER,
--     name TEXT,
--     phone_number TEXT,
--     passport_number INTEGER,
--     license_plate TEXT,
--     PRIMARY KEY(id)
-- );


-- Get the description from crime scene report given date/stree name
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND year = 2020
AND street = "Chamberlin Street";

-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time —
-- each of their interview transcripts mentions the courthouse.


-- Let's check the interview transcripts from that days too
SELECT id, name, transcript
FROM interviews
WHERE month = 7 AND day = 28 AND year = 2020;

-- 158 | Jose | “Ah,” said he, “I forgot that I had not seen you for some weeks. It is a little souvenir from the King of Bohemia in return for my assistance in the case of the Irene Adler papers.”
-- 159 | Eugene | “I suppose,” said Holmes, “that when Mr. Windibank came back from France he was very annoyed at your having gone to the ball.”
-- 160 | Barbara | “You had my note?” he asked with a deep harsh voice and a strongly marked German accent. “I told you that I would call.” He looked from one to the other of us, as if uncertain which to address.
-- 161 | Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- 162 | Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- 163 | Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
-- sqlite>


-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Each of the interview's mentions the courthouse
-- Check courthoues parking security tapes for cars leaving within ten minutes of theft
-- ATM on Fifer Street and saw the thief there withdrawing some money on same day
-- Call at time of leaving courthouse with accomplice
-- Theif planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.
-- The call was less than a minute.


-- Let's check courthouse_security_logs
SELECT hour, minute, activity, license_plate
FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28 AND
hour > 9 AND hour < 11 AND activity = "exit";

-- hour | minute | activity | license_plate
-- 10 | 16 | exit | 5P2BI95
-- 10 | 18 | exit | 94KL13X
-- 10 | 18 | exit | 6P58WS2
-- 10 | 19 | exit | 4328GD8
-- 10 | 20 | exit | G412CB7
-- 10 | 21 | exit | L93JTIZ
-- 10 | 23 | exit | 322W7JE
-- 10 | 23 | exit | 0NTHK55
-- 10 | 35 | exit | 1106N58


-- Let's check the license plates against the people database for our potential suspects
SELECT * FROM people WHERE license_plate IN
(SELECT license_plate
FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28 AND
hour > 9 AND hour < 11 AND minute < 25 AND activity = "exit");

-- id | name | phone_number | passport_number | license_plate
-- 221103 | Patrick | (725) 555-4692 | 2963008352 | 5P2BI95
-- 243696 | Amber | (301) 555-4174 | 7526138472 | 6P58WS2
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X


-- Let's check phone logs of suspects on that day; will need this later for idnetifying accomplice
SELECT * FROM phone_calls
WHERE year = 2020 AND month = 7 AND day = 28 AND caller IN
(SELECT phone_number FROM people WHERE license_plate IN
(SELECT license_plate
FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28 AND
hour > 9 AND hour < 11 AND activity = "exit"));

-- id | caller | receiver | year | month | day | duration
-- 221 | (130) 555-0289 | (996) 555-8899 | 2020 | 7 | 28 | 51
-- 224 | (499) 555-9472 | (892) 555-8872 | 2020 | 7 | 28 | 36
-- 230 | (301) 555-4174 | (711) 555-3007 | 2020 | 7 | 28 | 583
-- 233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
-- 236 | (367) 555-5533 | (344) 555-9601 | 2020 | 7 | 28 | 120
-- 245 | (367) 555-5533 | (022) 555-4052 | 2020 | 7 | 28 | 241
-- 251 | (499) 555-9472 | (717) 555-1342 | 2020 | 7 | 28 | 50
-- 254 | (286) 555-6063 | (676) 555-6554 | 2020 | 7 | 28 | 43
-- 255 | (770) 555-1861 | (725) 555-3243 | 2020 | 7 | 28 | 49
-- 257 | (725) 555-4692 | (821) 555-5262 | 2020 | 7 | 28 | 456
-- 284 | (286) 555-6063 | (310) 555-8568 | 2020 | 7 | 28 | 235
-- 285 | (367) 555-5533 | (704) 555-5790 | 2020 | 7 | 28 | 75


--- Then check atm_transactions morning of on Fifer street, withdrawl
SELECT * FROM atm_transactions
WHERE year = 2020 AND month = 7 AND day = 28 AND
transaction_type = "withdraw" AND
atm_location = "Fifer Street";

-- id | account_number | year | month | day | atm_location | transaction_type | amount
-- 246 | 28500762 | 2020 | 7 | 28 | Fifer Street | withdraw | 48
-- 264 | 28296815 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
-- 266 | 76054385 | 2020 | 7 | 28 | Fifer Street | withdraw | 60
-- 267 | 49610011 | 2020 | 7 | 28 | Fifer Street | withdraw | 50
-- 269 | 16153065 | 2020 | 7 | 28 | Fifer Street | withdraw | 80
-- 288 | 25506511 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
-- 313 | 81061156 | 2020 | 7 | 28 | Fifer Street | withdraw | 30
-- 336 | 26013199 | 2020 | 7 | 28 | Fifer Street | withdraw | 35


SELECT * FROM people WHERE id IN
(SELECT person_id FROM atm_transactions
JOIN bank_accounts
ON atm_transactions.account_number == bank_accounts.account_number
WHERE year = 2020 AND month = 7 AND day = 28 AND
transaction_type = "withdraw" AND
atm_location = "Fifer Street");

-- This is everyone who withdrew money on Fifer street on day of incident
-- id | name | phone_number | passport_number | license_plate
-- 395717 | Bobby | (826) 555-1652 | 9878712108 | 30G67EN
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 438727 | Victoria | (338) 555-6650 | 9586786673 | 8X428L0
-- 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58
-- 458378 | Roy | (122) 555-4581 | 4408372428 | QX4YZN3
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X


-- Cross-referencing with those who made a call from the coursehouse leaves us with the remainig
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X



--- Let's check for earliest flight out of fifyville tomorrow
SELECT * FROM flights WHERE origin_airport_id =
(SELECT id FROM airports WHERE city = "Fiftyville") AND
year = 2020 AND month = 7 AND day = 29 ORDER BY hour ASC;

-- id | origin_airport_id | destination_airport_id | year | month | day | hour | minute
-- 36 | 8 | 4 | 2020 | 7 | 29 | 8 | 20
-- 43 | 8 | 1 | 2020 | 7 | 29 | 9 | 30
-- 23 | 8 | 11 | 2020 | 7 | 29 | 12 | 15
-- 53 | 8 | 9 | 2020 | 7 | 29 | 15 | 20
-- 18 | 8 | 6 | 2020 | 7 | 29 | 16 | 0

SELECT full_name, city FROM airports WHERE id = 4;
-- full_name | city
-- Heathrow Airport | London

-- Earliest flight out of Fiftyville is 820AM to Heathrow Airport, London, flight_id = 36


-- Let's check passengers on the earliest flight out next
SELECT * FROM people WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id = 36);

-- id | name | phone_number | passport_number | license_plate
-- 395717 | Bobby | (826) 555-1652 | 9878712108 | 30G67EN
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 651714 | Edward | (328) 555-1152 | 1540955065 | 130LD9Z
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X
-- 953679 | Doris | (066) 555-9701 | 7214083635 | M51FA04


-- I need to cross-refernece the above names against my potential suspects list now...
-- i.e., those who made a call from the courthouse

-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X


-- Cross-reference with those who made a call on day of incident, note thta Danielle doesn't make a call,

-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X

-- Ernest is the primary suspect






-- id | caller | receiver | year | month | day | duration
-- 233 | (367) 555-5533 | (375) 555-8161 | 2020 | 7 | 28 | 45
-- 236 | (367) 555-5533 | (344) 555-9601 | 2020 | 7 | 28 | 120
-- 245 | (367) 555-5533 | (022) 555-4052 | 2020 | 7 | 28 | 241
-- 285 | (367) 555-5533 | (704) 555-5790 | 2020 | 7 | 28 | 75

SELECT * FROM people WHERE phone_number IN
("(375) 555-8161");

-- id | name | phone_number | passport_number | license_plate
-- 864400 | Berthold | (375) 555-8161 |  | 4V16VO0

-- Accomplice is Berthold