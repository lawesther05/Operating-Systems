#create a log file
log_file="script.log"

#function to make logging easier
log(){
    echo "$(date '+%F_%H-%M-%S') $1" >> "$log_file"
}

log "beginning of script"

#create a directory with the current date and time as it's name
dirname="$(date '+%F_%H-%M-%S')"
mkdir "$dirname"
log "main directory created"

#generate 10 subdirectories
for i in {101..110}; do
    filename="file$i"
    mkdir "$dirname/$filename"
    log "subdirectory $filename created"
    for j in {501..510}; do
        #make 10 txt files all containing the word "python"
        texts="Python"
        textfile="tuser$j"
        echo $texts > "$dirname/$filename/$textfile.txt"
        log "text file $textfile created"
    done
done