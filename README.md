NCAAHoopsPool
=============

NCAAHoopsPool is a PHP project that powers the [NCAAHoopsPool](http://www.ncaahoopspool.com) site. It was developed in PHP 5.5.12, Apache 2.4.9, PostgreSQL 11.2 and supports later versions. Currently deployed as a Heroku app at https://ncaahoopspool.herokuapp.com. This site is used to manage and participate in a season long college basketball pool with the following features:

- Weekly pool that runs during the NCAA basketball season (up to 18 weeks) 
- An administrator sets up each participant with their chosen Division 1 basketball team for the season, name and email address
- Players register with their email address and select a password
- Each week, the administrator selects one game that each participant's team is playing
- Players predict the winner of each weekly pool's games
- Weekly prize $$ is awarded to the player who correctly predicts the winner of the most games. Season long prizes are awarded to the top 3 finishers.
- As the administrator updates the result of each game, players can track that week's standings, see how other players picked games and track overall records

## Help and docs
- [NCAAHoopsPool PHP File Structure](docs/ncaahoopspool_file_structure.pdf)

## Installation (local)
To run a local version of this site:
1) Install [Wampserver](http://wampserver.aviatechno.net/)
2) Install [PostgreSQL](https://www.postgresql.org/download/)
3) Create a directory (e.g. 'ncaahoopspool') in your C:\wamp\www\ directory
4) Copy the contents of this repo into the directory you created in step #3
5) Create a new DB in postgresql
6) Run the SQL script [create_tables_CLEAN_2019-2020_Season.sql](sql/create_tables_CLEAN_2019-2020_Season.sql) to create needed tables and initialize with NCAA Division 1 team and weekly pool data (2019-2020 season)
7) Modify [misc.inc](includes/misc.inc) for your local DB credentials
8) In your browser, navigate to localhost\\'directory name from step #3'.index.php

To deploy to other platforms (e.g. Heroku), Composer will need to be installed. The only Composer dependency that this project supports is [Mailgun](https://www.mailgun.com/) for Forget Password & Admin Reminder emailing features.

## Contact Info
For questions or help using and/or setting up this site, contact:
Paul Maurer
paulrmaurer1@gmail.com