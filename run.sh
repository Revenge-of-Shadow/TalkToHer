./compile.sh > storage/comp_log.txt
./TalkToHer > storage/run_log.txt 
echo "Compilation."
cat storage/comp_log.txt
echo "Compilation end."
echo "Run."
cat storage/run_log.txt
echo "Run end."
