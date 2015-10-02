Drupency
========

Actency's default install profile

To download all the necessary files (incl. core and contrib) in the directory `drupency`, do

-	drush make build-drupency.make --prepare-install destination/to/new/site

- move into destination/to/new/site

You can now install the Drupency site using drush:

- drush site-install -y drupency --db-url=mysql://root:root@localhost/drupency --account-pass=admin --site-name="Drupency"
