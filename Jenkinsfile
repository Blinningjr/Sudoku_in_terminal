pipeline {
    agent any 
    stages {
        stage('Build') { 
            steps {
                sh 'gcc -I. main.c sudoku.c -o run'
            }
        }
    }
}
