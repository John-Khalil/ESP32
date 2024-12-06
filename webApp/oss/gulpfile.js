const gulp = require('gulp');
const sass = require('gulp-sass')(require('sass'));
const concat = require('gulp-concat');
const cleanCSS = require('gulp-clean-css');

function compileSass() {
  return gulp.src('./styles/**/*.scss') // Path to your SCSS files
    .pipe(sass().on('error', sass.logError))
    .pipe(concat('main.css'))
    .pipe(cleanCSS()) // Concatenate all CSS files into one main.css
    .pipe(gulp.dest('./public/css')); // Output directory for the main.css file
}

  gulp.task('compile-sass', compileSass);
  gulp.task('watch', () => {
    gulp.watch(['./styles/**/*.scss'], compileSass);
  })