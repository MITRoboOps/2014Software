FILE(REMOVE_RECURSE
  "CMakeFiles/robot_upstart_tmpl"
  "lib/robot_upstart/tmpl"
  "tmpl/start_gen"
  "tmpl/stop_gen"
  "tmpl/job_conf_gen"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/robot_upstart_tmpl.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
