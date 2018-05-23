const gulp = require('gulp')
const { exec } = require('child_process')

gulp.task('build:engine', (done) => {
  // build task
  console.log('building engine...')
  exec('make', (err, stdout, stderr) => {
    if (err) {
      console.error(err)
      return
    }
    console.log(stdout)
    done()
  })
})

gulp.task('build:game', (done) => {
  // since script files are the core of any game for this engine, just copy them over to resources
  console.log('copying game scripts...')
  exec('rsync -rvui --progress --exclude=.gitkeep resources/ bin/', (err, stdout, stderr) => {
    if (err) {
      console.error(err)
      return
    }
    console.log(stdout)
    done()
  })
})

// NOTE: one watch task per shell
gulp.task('watch:game', () => {
  // simple watch for all game script files
  gulp.watch('resources/scripts/*.lua', ['build:game'])
})

gulp.task('watch:engine', () => {
  // watch all engine files, header and implementation files
  gulp.watch('src/*.*', ['build:engine'])
})

gulp.task('watch', ['watch:engine', 'watch:game'], () => {})

gulp.task('build', ['build:engine', 'build:game'], () => {})

gulp.task('default', (done) => {
  console.log('done!')
  done()
} )