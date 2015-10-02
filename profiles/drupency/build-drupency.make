api = 2
core = 7.x

; Include the definition for how to build Drupal core directly, including patches:
includes[] = drupal-core.make

; Download the install profile and recursively build all its dependencies:
projects[drupency][type] = "profile"
projects[drupency][download][type] = "git"
projects[drupency][download][url] = "https://github.com/Actency/drupency.git"
projects[drupency][download][branch] = "develop"