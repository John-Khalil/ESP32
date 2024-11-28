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
    public class AppDbContext : DbContext
    {
        public DbSet<Models.Settings> Settings{ get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlite("Data Source=dataBase.db");
        }
    }
}