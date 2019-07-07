pipeline {
    agent any 
    stages {
        stage('Static analysis') {
              steps {
                sh 'cppcheck --xml --xml-version=2 /var/lib/jenkins/workspace/Sudoku_in_terminal 2> cppcheck.xml'
              }
        }
        stage('Build') { 
            steps {
                sh 'gcc -I. main.c sudoku.c -o run'
            }
        }
    }
}
