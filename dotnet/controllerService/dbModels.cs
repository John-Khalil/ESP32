using Microsoft.EntityFrameworkCore;
namespace EFCore.Models
{
    public class Settings
    { 
        public int Id { get; set; }
        public int Name { get; set; }
    }


}

namespace EFCore
{
    public class DB : DbContext
    {
        public DbSet<Models.Settings> Settings{ get; set; }
        static DB()
        {
            using (var db = new DB())
            {
                db.Database.EnsureCreated();
            }
        }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlite("Data Source=dataBase.db");
        }

        // Create
        public static void Create<T>(T entity) where T : class
        {
            using (var db = new DB())
            {
                db.Set<T>().Add(entity);    // Use Set<T>() instead of trying to access DbSet directly
                db.SaveChanges();
            }
        }

        // Read (Get All)
        public static List<T> GetAll<T>() where T : class
        {
            using (var db = new DB())
            {
                return db.Set<T>().ToList();
            }
        }

        // Read (Get By Id)
        public static T GetById<T>(int id) where T : class
        {
            using (var db = new DB())
            {
                return db.Set<T>().Find(id);
            }
        }

        // Update
        public static void Update<T>(T entity) where T : class
        {
            using (var db = new DB())
            {
                db.Set<T>().Update(entity);
                db.SaveChanges();
            }
        }

        // Delete
        public static void Delete<T>(T entity) where T : class
        {
            using (var db = new DB())
            {
                db.Set<T>().Remove(entity);
                db.SaveChanges();
            }
        }

        // Delete by Id
        public static bool DeleteById<T>(int id) where T : class
        {
            using (var db = new DB())
            {
                var entity = db.Set<T>().Find(id);
                if (entity == null)
                    return false;
                
                db.Set<T>().Remove(entity);
                db.SaveChanges();
                return true;
            }
        }

        // Additional useful methods

        // Get with filtering
        public static List<T> GetWhere<T>(System.Linq.Expressions.Expression<Func<T, bool>> predicate) where T : class
        {
            using (var db = new DB())
            {
                return db.Set<T>().Where(predicate).ToList();
            }
        }

        // Check if exists
        public static bool Exists<T>(int id) where T : class
        {
            using (var db = new DB())
            {
                return db.Set<T>().Find(id) != null;
            }
        }

        // Count
        public static int Count<T>() where T : class
        {
            using (var db = new DB())
            {
                return db.Set<T>().Count();
            }
        }

    }

}